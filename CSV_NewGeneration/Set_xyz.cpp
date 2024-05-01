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
	std::ofstream trainset_attitude_roll("trainset_attitude_roll.csv");
	std::ofstream trainset_attitude_pitch("trainset_attitude_pitch.csv");
	std::ofstream trainset_attitude_yaw("trainset_attitude_yaw.csv");
	std::ofstream trainset_gravity_x("trainset_gravity_x.csv");
	std::ofstream trainset_gravity_y("trainset_gravity_y.csv");
	std::ofstream trainset_gravity_z("trainset_gravity_z.csv");
	std::ofstream trainset_rotation_rate_x("trainset_rotation_rate_x.csv");
	std::ofstream trainset_rotation_rate_y("trainset_rotation_rate_y.csv");
	std::ofstream trainset_rotation_rate_z("trainset_rotation_rate_z.csv");
	std::ofstream trainset_user_acceleration_x("trainset_user_acceleration_x.csv");
	std::ofstream trainset_user_acceleration_y("trainset_user_acceleration_y.csv");
	std::ofstream trainset_user_acceleration_z("trainset_user_acceleration_z.csv");

	std::ofstream testset_attitude_roll("testset_attitude_roll.csv");
	std::ofstream testset_attitude_pitch("testset_attitude_pitch.csv");
	std::ofstream testset_attitude_yaw("testset_attitude_yaw.csv");
	std::ofstream testset_gravity_x("testset_gravity_x.csv");
	std::ofstream testset_gravity_y("testset_gravity_y.csv");
	std::ofstream testset_gravity_z("testset_gravity_z.csv");
	std::ofstream testset_rotation_rate_x("testset_rotation_rate_x.csv");
	std::ofstream testset_rotation_rate_y("testset_rotation_rate_y.csv");
	std::ofstream testset_rotation_rate_z("testset_rotation_rate_z.csv");
	std::ofstream testset_user_acceleration_x("testset_user_acceleration_x.csv");
	std::ofstream testset_user_acceleration_y("testset_user_acceleration_y.csv");
	std::ofstream testset_user_acceleration_z("testset_user_acceleration_z.csv");

	create_header_xyz(trainset_attitude_roll, TRAINSET_COLUMNS);
	create_header_xyz(trainset_attitude_pitch, TRAINSET_COLUMNS);
	create_header_xyz(trainset_attitude_yaw, TRAINSET_COLUMNS);
	create_header_xyz(trainset_gravity_x, TRAINSET_COLUMNS);
	create_header_xyz(trainset_gravity_y, TRAINSET_COLUMNS);
	create_header_xyz(trainset_gravity_z, TRAINSET_COLUMNS);
	create_header_xyz(trainset_rotation_rate_x, TRAINSET_COLUMNS);
	create_header_xyz(trainset_rotation_rate_y, TRAINSET_COLUMNS);
	create_header_xyz(trainset_rotation_rate_z, TRAINSET_COLUMNS);
	create_header_xyz(trainset_user_acceleration_x, TRAINSET_COLUMNS);
	create_header_xyz(trainset_user_acceleration_y, TRAINSET_COLUMNS);
	create_header_xyz(trainset_user_acceleration_z, TRAINSET_COLUMNS);

	create_header_xyz(testset_attitude_roll, TESTSET_COLUMNS);
	create_header_xyz(testset_attitude_pitch, TESTSET_COLUMNS);
	create_header_xyz(testset_attitude_yaw, TESTSET_COLUMNS);
	create_header_xyz(testset_gravity_x, TESTSET_COLUMNS);
	create_header_xyz(testset_gravity_y, TESTSET_COLUMNS);
	create_header_xyz(testset_gravity_z, TESTSET_COLUMNS);
	create_header_xyz(testset_rotation_rate_x, TESTSET_COLUMNS);
	create_header_xyz(testset_rotation_rate_y, TESTSET_COLUMNS);
	create_header_xyz(testset_rotation_rate_z, TESTSET_COLUMNS);
	create_header_xyz(testset_user_acceleration_x, TESTSET_COLUMNS);
	create_header_xyz(testset_user_acceleration_y, TESTSET_COLUMNS);
	create_header_xyz(testset_user_acceleration_z, TESTSET_COLUMNS);

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

		trainset_attitude_roll << (directory_index + 1) << DELIMITER << gender << DELIMITER << file_index;
		trainset_attitude_pitch << (directory_index + 1) << DELIMITER << gender << DELIMITER << file_index;
		trainset_attitude_yaw << (directory_index + 1) << DELIMITER << gender << DELIMITER << file_index;
		trainset_gravity_x << (directory_index + 1) << DELIMITER << gender << DELIMITER << file_index;
		trainset_gravity_y << (directory_index + 1) << DELIMITER << gender << DELIMITER << file_index;
		trainset_gravity_z << (directory_index + 1) << DELIMITER << gender << DELIMITER << file_index;
		trainset_rotation_rate_x << (directory_index + 1) << DELIMITER << gender << DELIMITER << file_index;
		trainset_rotation_rate_y << (directory_index + 1) << DELIMITER << gender << DELIMITER << file_index;
		trainset_rotation_rate_z << (directory_index + 1) << DELIMITER << gender << DELIMITER << file_index;
		trainset_user_acceleration_x << (directory_index + 1) << DELIMITER << gender << DELIMITER << file_index;
		trainset_user_acceleration_y << (directory_index + 1) << DELIMITER << gender << DELIMITER << file_index;
		trainset_user_acceleration_z << (directory_index + 1) << DELIMITER << gender << DELIMITER << file_index;
		
		std::string line;
		uint64_t trainset_explored = 0;
		while (trainset_explored < TRAINSET_COLUMNS && std::getline(current_file, line)) {
			RawLine rawline;
			std::istringstream iss(line);
			extract_rawline(rawline, iss);

			trainset_attitude_roll << DELIMITER << rawline.attitude_roll;
			trainset_attitude_pitch << DELIMITER << rawline.attitude_pitch;
			trainset_attitude_yaw << DELIMITER << rawline.attitude_yaw;
			trainset_gravity_x << DELIMITER << rawline.gravity_x;
			trainset_gravity_y << DELIMITER << rawline.gravity_y;
			trainset_gravity_z << DELIMITER << rawline.gravity_z;
			trainset_rotation_rate_x << DELIMITER << rawline.rotation_rate_x;
			trainset_rotation_rate_y << DELIMITER << rawline.rotation_rate_y;
			trainset_rotation_rate_z << DELIMITER << rawline.rotation_rate_z;
			trainset_user_acceleration_x << DELIMITER << rawline.user_acceleration_x;
			trainset_user_acceleration_y << DELIMITER << rawline.user_acceleration_y;
			trainset_user_acceleration_z << DELIMITER << rawline.user_acceleration_z;
			
			trainset_explored++;
		}

		trainset_attitude_roll << std::endl;
		trainset_attitude_pitch << std::endl;
		trainset_attitude_yaw << std::endl;
		trainset_gravity_x << std::endl;
		trainset_gravity_y << std::endl;
		trainset_gravity_z << std::endl;
		trainset_rotation_rate_x << std::endl;
		trainset_rotation_rate_y << std::endl;
		trainset_rotation_rate_z << std::endl;
		trainset_user_acceleration_x << std::endl;
		trainset_user_acceleration_y << std::endl;
		trainset_user_acceleration_z << std::endl;

		if (trainset_explored == TRAINSET_COLUMNS) {
			testset_attitude_roll << (directory_index + 1) << DELIMITER << gender << DELIMITER << file_index;
			testset_attitude_pitch << (directory_index + 1) << DELIMITER << gender << DELIMITER << file_index;
			testset_attitude_yaw << (directory_index + 1) << DELIMITER << gender << DELIMITER << file_index;
			testset_gravity_x << (directory_index + 1) << DELIMITER << gender << DELIMITER << file_index;
			testset_gravity_y << (directory_index + 1) << DELIMITER << gender << DELIMITER << file_index;
			testset_gravity_z << (directory_index + 1) << DELIMITER << gender << DELIMITER << file_index;
			testset_rotation_rate_x << (directory_index + 1) << DELIMITER << gender << DELIMITER << file_index;
			testset_rotation_rate_y << (directory_index + 1) << DELIMITER << gender << DELIMITER << file_index;
			testset_rotation_rate_z << (directory_index + 1) << DELIMITER << gender << DELIMITER << file_index;
			testset_user_acceleration_x << (directory_index + 1) << DELIMITER << gender << DELIMITER << file_index;
			testset_user_acceleration_y << (directory_index + 1) << DELIMITER << gender << DELIMITER << file_index;
			testset_user_acceleration_z << (directory_index + 1) << DELIMITER << gender << DELIMITER << file_index;

			std::string line;
			uint64_t testset_explored = 0;
			while (testset_explored < TESTSET_COLUMNS && std::getline(current_file, line)) {
				RawLine rawline;
				std::istringstream iss(line);
				extract_rawline(rawline, iss);
				
				testset_attitude_roll << DELIMITER << rawline.attitude_roll;
				testset_attitude_pitch << DELIMITER << rawline.attitude_pitch;
				testset_attitude_yaw << DELIMITER << rawline.attitude_yaw;
				testset_gravity_x << DELIMITER << rawline.gravity_x;
				testset_gravity_y << DELIMITER << rawline.gravity_y;
				testset_gravity_z << DELIMITER << rawline.gravity_z;
				testset_rotation_rate_x << DELIMITER << rawline.rotation_rate_x;
				testset_rotation_rate_y << DELIMITER << rawline.rotation_rate_y;
				testset_rotation_rate_z << DELIMITER << rawline.rotation_rate_z;
				testset_user_acceleration_x << DELIMITER << rawline.user_acceleration_x;
				testset_user_acceleration_y << DELIMITER << rawline.user_acceleration_y;
				testset_user_acceleration_z << DELIMITER << rawline.user_acceleration_z;
				
				testset_explored++;
			}

			testset_attitude_roll << std::endl;
			testset_attitude_pitch << std::endl;
			testset_attitude_yaw << std::endl;
			testset_gravity_x << std::endl;
			testset_gravity_y << std::endl;
			testset_gravity_z << std::endl;
			testset_rotation_rate_x << std::endl;
			testset_rotation_rate_y << std::endl;
			testset_rotation_rate_z << std::endl;
			testset_user_acceleration_x << std::endl;
			testset_user_acceleration_y << std::endl;
			testset_user_acceleration_z << std::endl;
		}

		file_index++;
	});
}