#include "Global.hpp"

int main() {
	std::fstream combination(COMBINATION_FILENAME, std::ios::out);

	combination <<
		"Combination" << DELIMITER <<
		"Extreme Function" << DELIMITER <<
		"Trainset Columns" << DELIMITER <<
		"Testset Columns" << DELIMITER <<
		"Result" <<
		std::endl;

	std::unordered_map<std::string, ExtremeFunction> extremes_func = {
		{"noextreme", no_extreme},
		{"isextreme", is_extreme},
		{"isextremez", is_extreme_z}
	};

	std::vector<uint64_t> columns_volums = {
		TRAINSET_COLUMNS,
		TRAINSET_COLUMNS * 2,
		TRAINSET_COLUMNS * 3,
	};

	std::vector<double> testset_proportions = {
		1,
		0.1,
		0.5
	};

	const auto max_names = static_cast<size_t>(RawColumnName::SIZE);
	for (size_t name_index = 1; name_index < pow(2, max_names); name_index++) {
		const std::bitset<static_cast<uint64_t>(RawColumnName::SIZE)> flags(name_index);
		std::vector<RawColumnName> extract;
		to_columns(flags, extract);

		for (auto [extreme_name, extreme_func] : extremes_func) {
			for (auto column : columns_volums) {
				for (auto proportion : testset_proportions) {
					auto start = std::chrono::system_clock::now();

					RunParameter run = RunParameter(column, column * proportion, extract, extreme_func);

					std::string combination_name =
						flags.to_string() + "_" +
						extreme_name + "_" +
						std::to_string(column) + "_" +
						std::to_string((uint64_t)(column * proportion));

					std::cout << "Combination: " << combination_name << std::endl;

					const std::string pattern_filename = "pattern_" + combination_name + ".csv";
					const std::string testset_filename = "testset_" + combination_name + ".csv";
					const std::string trainset_filename = "trainset_" + combination_name + ".csv";
					const std::string evaluation_filename = "evalution_" + combination_name + ".csv";

					auto result = do_run(run, trainset_filename, testset_filename, pattern_filename, evaluation_filename);
					view_result(result, evaluation_filename);

					auto performance = get_performance(result);
					
					auto end = std::chrono::system_clock::now();
					std::chrono::duration<double> elapsed = end - start;
					std::cout << "Took: " << elapsed.count() << std::endl;

					combination <<
						flags.to_string() << DELIMITER <<
						extreme_name << DELIMITER <<
						column << DELIMITER <<
						column * proportion << DELIMITER <<
						performance <<
						std::endl;
				}
			}
		}
	}
}