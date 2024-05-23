#include "Global.hpp"

void for_file(const fs::path& directory, std::function<void(fs::path)> on_file) {
	for (const auto& entry : fs::directory_iterator(directory)) {
		const auto& current_path = entry.path();
		if (fs::is_regular_file(current_path)) {
			on_file(current_path);
		}
		else if (fs::is_directory(current_path)) {
			for_file(current_path, on_file);
		}
	}
}

void extract_rawline(RawLine& rawline, std::istringstream& iss) {
	char delimiter;

	iss >>
		rawline.id >> delimiter >>
		rawline.attitude_roll >> delimiter >>
		rawline.attitude_pitch >> delimiter >>
		rawline.attitude_yaw >> delimiter >>
		rawline.gravity_x >> delimiter >>
		rawline.gravity_y >> delimiter >>
		rawline.gravity_z >> delimiter >>
		rawline.rotation_rate_x >> delimiter >>
		rawline.rotation_rate_y >> delimiter >>
		rawline.rotation_rate_z >> delimiter >>
		rawline.user_acceleration_x >> delimiter >>
		rawline.user_acceleration_y >> delimiter >>
		rawline.user_acceleration_z >> delimiter;
};

void extract_setline_core(SetLine& setline, std::istringstream& iss) {
	char delimiter;
	uint64_t movement;

	iss >>
		movement >> delimiter >>
		setline.person_id >> delimiter >>
		setline.gender >> delimiter;

	setline.movement_type = static_cast<MovementType>(movement);
}

void extract_subjectline(SubjectLine& subjectline, std::istringstream& iss) {
	char delimiter;

	iss >>
		subjectline.code >> delimiter >>
		subjectline.weight >> delimiter >>
		subjectline.height >> delimiter >>
		subjectline.age >> delimiter >>
		subjectline.gender;
}

[[nodiscard]]
bool is_extreme_z(const double value, const double average, const double std) {
	const double z_score = (value - average) / std;
	return z_score > average + (3 * std) || z_score < average - (3 * std);
}

[[nodiscard]]
bool is_extreme(const double value, const double average, const double std) {
	return value > average + (3 * std) || value < average - (3 * std);
}

[[nodiscard]]
bool no_extreme(const double value, const double average, const double std) {
	return false;
}

[[nodiscard]]
size_t find_directory_type(
	const std::string& directory_name
) {
	size_t pair_index = 0;
	while (pair_index < MOVEMENT_REGEX.size() && !std::regex_match(directory_name, MOVEMENT_REGEX[pair_index].first)) {
		pair_index++;
	}
	return pair_index;
}

[[nodiscard]]
uint64_t find_gender(const uint64_t person_id) {
	std::ifstream subjects(SUBJECT_FILEPATH);
	if (!subjects.good()) {
		std::cerr << "Error while opening: " << SUBJECT_FILEPATH << std::endl;
	}

	SubjectLine subjectline{ .code = 0 };

	std::string header;
	std::getline(subjects, header);

	std::string line;
	while (std::getline(subjects, line) && subjectline.code < person_id) {
		std::istringstream iss(line);
		extract_subjectline(subjectline, iss);
	}

	if (subjectline.code == 0) {
		std::cerr << "Couldnt find subject of id: " << person_id << std::endl;
	}

	return subjectline.gender;
}

void create_header(std::fstream& output_file, const size_t columns_count) {
	output_file << "Mouvement" << DELIMITER << "Gender" << DELIMITER << "Index";
	for (size_t index = 0; index < columns_count; index++) {
		output_file << DELIMITER << "Vacc";
	}
	output_file << std::endl;
}

void view_result(
	const std::vector<std::vector<double>>& result,
	const std::string evalution_filename
) {
	std::fstream evaluation(evalution_filename, std::ios::out);

	evaluation <<
		"Mouvement" << DELIMITER <<
		"1" << DELIMITER <<
		"2" << DELIMITER <<
		"3" << DELIMITER <<
		"4" << DELIMITER <<
		"5" << DELIMITER <<
		"6" << DELIMITER << std::endl;

	double total_right = 0;
	double total = 0;
	for (size_t eval_index = 0; eval_index < result.size(); eval_index++) {
		std::cout << (eval_index + 1) << " | ";
		evaluation << (eval_index + 1) << DELIMITER;

		double mov_total = 0;
		for (size_t guess_index = 0; guess_index < result[eval_index].size(); guess_index++) {
			double guesses = result[eval_index][guess_index];
			std::cout << guesses << " | ";
			evaluation << guesses << DELIMITER;
			mov_total += guesses;
			total += guesses;
		}
		evaluation << std::endl;

		double mov_right = result[eval_index][eval_index];
		total_right += mov_right;

		double succes_rate = ((double)mov_right / mov_total) * 100;

		std::cout
			<< "Total : " << mov_total << " | "
			<< "Accuracy: " << succes_rate << "%" << std::endl;
	}

	std::cout
		<< "Total: "
		<< total_right << " | "
		<< total << " | "
		<< std::round((total_right / total) * 100) << "%"
		<< std::endl;
}

void to_columns(
	const std::bitset<static_cast<RawColumnName>(RawColumnName::SIZE)>& bits,
	std::vector<RawColumnName>& columns
) {
	for (size_t flag_index = 0; flag_index < bits.size(); flag_index++) {
		bool bit = bits.test(flag_index);
		if (bit) {
			columns.push_back(static_cast<RawColumnName>(flag_index));
		}
	}
}

void to_bitset(
	const std::vector<RawColumnName>& columns,
	std::bitset<static_cast<RawColumnName>(RawColumnName::SIZE)>& bits
) {
	for (size_t col_index = 0; col_index < columns.size(); col_index++) {
		auto column = columns[col_index];
		bits.set(static_cast<uint64_t>(column));
	}
}

double get_performance(const std::vector<std::vector<double>>& results) {
	double total = 0;
	double total_right = 0;
	for (size_t eval_index = 0; eval_index < results.size(); eval_index++) {
		total_right += results[eval_index][eval_index];

		for (size_t guess_index = 0; guess_index < results[eval_index].size(); guess_index++) {
			total += results[eval_index][guess_index];
		}
	}
	return double(total_right) / total;
}