#include "global.hpp"

void create_evaluation(
	std::vector<std::vector<double>>& testset_matrix,
	std::vector<std::vector<double>>& pattern_matrix,
	std::vector<std::vector<double>>& evaluation_matrix,
#if EVALUTIONFILE
	std::fstream& evaluation,
#endif
	std::unordered_map<uint64_t, uint64_t>& sensitives_files,
	const RunParameter& run,
	const double max_value
) {
	for (size_t testset_row_index = 0; testset_row_index < 360; testset_row_index++) {
		auto is_sensitive = sensitives_files.find(testset_row_index) != sensitives_files.end();
		int64_t testset_lines = is_sensitive ? (int64_t)(sensitives_files.at(testset_row_index) - run.trainset_col) : run.testset_col;
		if (testset_lines > 0) {
			double distance_min = max_value;
			uint64_t movement_min = static_cast<uint64_t>(MovementType::SIZE);
			for (size_t pattern_row_index = 0; pattern_row_index < 6; pattern_row_index++) {
				double total = 0;
				for (size_t min_column_index = 1; min_column_index < std::min(run.trainset_col, run.testset_col); min_column_index++) {
					auto testset_value = testset_matrix[testset_row_index][min_column_index];
					auto pattern_value = pattern_matrix[pattern_row_index][min_column_index];
					total += std::pow(testset_value - pattern_value, 2);
				}
				double distance = std::sqrt(total);
				if (distance < distance_min) {
					distance_min = distance;
					movement_min = pattern_row_index;
				}
			}
			const uint64_t movement_index = (uint64_t)(testset_matrix[testset_row_index][0] - 1);
			evaluation_matrix[movement_index][movement_min]++;
		}
	}

#if EVALUTIONFILE
	for (size_t row_index = 0; row_index < 6; row_index++) {
		evaluation << (row_index + 1) << DELIMITER;
		for (size_t column_index = 0; column_index < 6; column_index++) {
			auto value = evaluation_matrix[row_index][column_index];
			evaluation << value << DELIMITER;
		}
		evaluation << std::endl;
	}
#endif
}