#include "Global.hpp"

void create_header_xyz(
	std::fstream& output_file, 
	const size_t columns_count
) {
	output_file << "Mouvement" << DELIMITER << "Gender" << DELIMITER << "Index";
	for (size_t index = 0; index < columns_count; index++) {
		output_file << DELIMITER << "Vacc";
	}
	output_file << std::endl;
}


[[nodiscard]]
uint64_t create_set_xyz(
	std::fstream& current_file,
	std::fstream& output_file,
	const uint64_t line_count,
	std::function<double(RawLine)> get_acc
) {
	uint64_t lines_explored = 0;

	std::string line;
	while (lines_explored < line_count && std::getline(current_file, line)) {
		RawLine rawline;
		std::istringstream iss(line);
		extract_rawline(rawline, iss);

		output_file << DELIMITER << get_acc(rawline);
		
		lines_explored++;
	}

	output_file << std::endl;
	return lines_explored;
}

ProcessError set_xyz(
	std::fstream& trainset,
	std::fstream& testset,
	std::function<double(RawLine)> get_acc
) {
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
		const uint64_t gender = find_gender(person_id);

		std::fstream current_file(current_path);
		if (!current_file.is_open())
			return COUDLNT_OPEN_FILE;

		std::string header;
		std::getline(current_file, header);

		trainset << (directory_index + 1) << DELIMITER << gender << DELIMITER << file_index;
		uint64_t trainset_lines = create_set_xyz(current_file, trainset, TRAINSET_COLUMNS, get_acc);

		if (trainset_lines == TRAINSET_COLUMNS) {
			testset << (directory_index + 1) << DELIMITER << gender << DELIMITER << file_index;
			uint64_t testset_lines = create_set_xyz(current_file, testset, TESTSET_COLUMNS, get_acc);
		}

		file_index++;
	});

	return NO_ERROR;
}