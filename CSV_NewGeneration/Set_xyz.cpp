#include "Global.hpp"

void create_header_xyz(
	std::ofstream& output_file, 
	const size_t columns_count
) {
	output_file << "Mouvement" << DELIMITER << "Gender" << DELIMITER << "Index";
	for (size_t index = 0; index < columns_count; index++) {
		output_file << DELIMITER << "Vacc";
	}
	output_file << std::endl;
}

void set_xyz() {
	std::ofstream trainset_x("trainset_x.csv");
	std::ofstream trainset_y("trainset_y.csv");
	std::ofstream trainset_z("trainset_z.csv");

	std::ofstream testset_x("testset_x.csv");
	std::ofstream testset_y("testset_y.csv");
	std::ofstream testset_z("testset_z.csv");

	create_header_xyz(trainset_x, TRAINSET_COLUMNS);
	create_header_xyz(trainset_y, TRAINSET_COLUMNS);
	create_header_xyz(trainset_z, TRAINSET_COLUMNS);

	create_header_xyz(testset_x, TESTSET_COLUMNS);
	create_header_xyz(testset_y, TESTSET_COLUMNS);
	create_header_xyz(testset_z, TESTSET_COLUMNS);

	uint64_t file_index = 1;
	for_file(DATA_FOLDERPATH, [&](const fs::path current_path) {
		const std::string directory_name = current_path.parent_path().filename().string();

		const size_t directory_index = find_directory_type(directory_name);
		if (directory_index == MOVEMENT_REGEX.size())
			std::cerr << "Couldnt find directory of type: " << directory_index << std::endl;

		const std::string filepath_name = current_path.string();
		std::smatch matches;
		if (!std::regex_search(filepath_name, matches, PERSON_FILE_REGEX))
			std::cerr << "Couldnt find person" << std::endl;

		const MovementType directory_type = MOVEMENT_REGEX[directory_index].second;
		const uint64_t person_id = std::stoull(matches[1].str());
		const uint64_t gender = find_gender(person_id);

		std::fstream current_file(current_path);
		if (!current_file.is_open())
			std::cerr << "Couldnt open file: " << current_path << std::endl;

		std::string header;
		std::getline(current_file, header);

		trainset_x << (directory_index + 1) << DELIMITER << gender << DELIMITER << file_index;
		trainset_y << (directory_index + 1) << DELIMITER << gender << DELIMITER << file_index;
		trainset_z << (directory_index + 1) << DELIMITER << gender << DELIMITER << file_index;
		
		std::string line;
		uint64_t trainset_explored = 0;
		while (trainset_explored < TRAINSET_COLUMNS && std::getline(current_file, line)) {
			RawLine rawline;
			std::istringstream iss(line);
			extract_rawline(rawline, iss);

			trainset_x << DELIMITER << rawline.user_acceleration_x;
			trainset_y << DELIMITER << rawline.user_acceleration_y;
			trainset_z << DELIMITER << rawline.user_acceleration_z;
			
			trainset_explored++;
		}

		trainset_x << std::endl;
		trainset_y << std::endl;
		trainset_z << std::endl;

		if (trainset_explored == TRAINSET_COLUMNS) {
			testset_x << (directory_index + 1) << DELIMITER << gender << DELIMITER << file_index;
			testset_y << (directory_index + 1) << DELIMITER << gender << DELIMITER << file_index;
			testset_z << (directory_index + 1) << DELIMITER << gender << DELIMITER << file_index;
			
			std::string line;
			uint64_t testset_explored = 0;
			while (testset_explored < TESTSET_COLUMNS && std::getline(current_file, line)) {
				RawLine rawline;
				std::istringstream iss(line);
				extract_rawline(rawline, iss);
				
				testset_x << DELIMITER << rawline.user_acceleration_x;
				testset_y << DELIMITER << rawline.user_acceleration_y;
				testset_z << DELIMITER << rawline.user_acceleration_z;
				
				testset_explored++;
			}

			testset_x << std::endl;
			testset_y << std::endl;
			testset_z << std::endl;
		}

		file_index++;
	});
}