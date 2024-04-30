#include "Global.hpp"

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
uint64_t find_gender(
	std::fstream& subjects, 
	const uint64_t person_id
) {
	uint64_t code = 0;
	uint64_t weight;
	uint64_t height;
	uint64_t age;
	uint64_t gender = 0;

	char delimiter;

	subjects.clear();
	subjects.seekg(0, std::ios::beg);

	std::string header;
	std::getline(subjects, header);

	std::string line;
	while (std::getline(subjects, line) && code < person_id) {
		std::istringstream iss(line);

		iss >>
			code >> delimiter >>
			weight >> delimiter >>
			height >> delimiter >>
			age >> delimiter >>
			gender;
	}

	return gender;
}

void create_header(std::fstream& output_file, const size_t columns_count) {
	output_file << "Mouvement" << DELIMITER << "Gender" << DELIMITER << "Index";
	for (size_t index = 0; index < columns_count; index++) {
		output_file << DELIMITER << "Vacc";
	}
	output_file << std::endl;
}

void filter_justone(
	const double acceleration_x, 
	const double acceleration_y,
	const double acceleration_z,
	std::function<bool(double, double, double)> extreme_func,
	std::fstream& output_file
) {
	const bool extreme_x = extreme_func(acceleration_x, AVERAGE_X, STANDARD_DEVIATION_X);
	const bool extreme_y = extreme_func(acceleration_y, AVERAGE_Y, STANDARD_DEVIATION_Y);
	const bool extreme_z = extreme_func(acceleration_z, AVERAGE_Z, STANDARD_DEVIATION_Z);

	if (!extreme_x && !extreme_y && !extreme_z) {
		double acceleration = sqrt(pow(acceleration_x, 2) + pow(acceleration_y, 2) + pow(acceleration_z, 2));
		output_file << DELIMITER << acceleration;
	}
}

void filter_toaverage(
	double acceleration_x,
	double acceleration_y,
	double acceleration_z,
	std::function<bool(double, double, double)> extreme_func,
	std::fstream& output_file
) {
	const bool extreme_x = extreme_func(acceleration_x, AVERAGE_X, STANDARD_DEVIATION_X);
	const bool extreme_y = extreme_func(acceleration_y, AVERAGE_Y, STANDARD_DEVIATION_Y);
	const bool extreme_z = extreme_func(acceleration_z, AVERAGE_Z, STANDARD_DEVIATION_Z);

	if (extreme_x) {
		acceleration_x = AVERAGE_X;
	}

	if (extreme_y) {
		acceleration_y = AVERAGE_Y;
	}

	if (extreme_z) {
		acceleration_z = AVERAGE_Z;
	}

	double acceleration = sqrt(pow(acceleration_x, 2) + pow(acceleration_y, 2) + pow(acceleration_z, 2));
	output_file << DELIMITER << acceleration;
}

[[nodiscard]] 
uint64_t create_set(
	std::fstream& current_file, 
	const uint64_t line_count, 
	std::fstream& output_file,
	std::function<bool(double, double, double)> extreme_func
) {
	uint64_t lines_explored = 0;

	std::string line;
	while (lines_explored < line_count && std::getline(current_file, line)) {
		RawLine rawline;
		std::istringstream iss(line);
		extract_rawline(rawline, iss);

		filter_justone(
			rawline.user_acceleration_x,
			rawline.user_acceleration_y,
			rawline.user_acceleration_z,
			extreme_func,
			output_file
		);

		lines_explored++;
	}
	output_file << std::endl;
	
	return lines_explored;
}

ProcessError set(
	std::fstream& subjects, 
	std::fstream& trainset, 
	std::fstream& testset,
	std::function<bool(double, double, double)> extreme_func
) {
	std::unordered_map<uint64_t, uint64_t> gender_map;

	create_header(trainset, TRAINSET_COLUMNS);
	create_header(testset, TESTSET_COLUMNS);

	uint64_t file_index = 1;
	for_file(DATA_FOLDERPATH, [&](const fs::path current_path) {
		const std::string directory_name = current_path.parent_path().filename().string();

		const size_t directory_index = find_directory_type(directory_name);
		if (directory_index == MOVEMENT_REGEX.size())
			return DIRECTORY_TYPE_NOT_FOUND;

		const std::string filepath_name = current_path.string();
		std::smatch matches;
		if (!std::regex_search(filepath_name, matches, PERSON_FILE_REGEX))
			return PERSON_FILE_NOT_FOUND;

		const MovementType directory_type = MOVEMENT_REGEX[directory_index].second;
		const uint64_t person_id = std::stoull(matches[1].str());
		const uint64_t gender = find_gender(subjects, person_id);

		std::fstream current_file(current_path);
		if (!current_file.is_open())
			return COUDLNT_OPEN_FILE;

		std::string header;
		std::getline(current_file, header);

		trainset << (directory_index + 1) << DELIMITER << gender << DELIMITER << file_index;
		uint64_t trainset_lines = create_set(current_file, TRAINSET_COLUMNS, trainset, extreme_func);

		if (trainset_lines == TRAINSET_COLUMNS) {
			testset << (directory_index + 1) << DELIMITER << gender << DELIMITER << file_index;
			uint64_t testset_lines = create_set(current_file, TESTSET_COLUMNS, testset, extreme_func);
		}
		
		file_index++;
	});

	return NO_ERROR;
}

ProcessError phase_zero() {
	std::fstream subjects(SUBJECT_FILEPATH, std::ios::in);
	if (!subjects.is_open()) 
		return COUDLNT_OPEN_FILE;
	
	std::fstream trainset(TRAINSET_FILENAME, std::ios::out);
	if (!trainset.is_open()) 
		return COUDLNT_OPEN_FILE;
	
	std::fstream testset(TESTSET_FILENAME, std::ios::out);
	if (!testset.is_open())
		return COUDLNT_OPEN_FILE;

	return set(subjects, trainset, testset, is_extreme);
}