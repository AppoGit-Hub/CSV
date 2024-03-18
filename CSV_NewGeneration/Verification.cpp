#include "Global.hpp"

inline const std::string DELIMITER = ",";
inline const std::string BASE_FOLDER = "archive";
inline const std::string DATA_FOLDER = "data";

static void proccess_file(const fs::path& current_path) {
	std::ifstream current_file(current_path);
	
	std::string header;
	std::getline(current_file, header);

	uint64_t total_lines = 0;
	uint64_t last_id = 0;
	bool line_error = false;
	char delimiter;

	uint64_t id;
	double attitude_roll;
	double attitude_pitch;
	double attitude_yaw;
	double gravity_x;
	double gravity_y;
	double gravity_z;
	double rotationRate_x;
	double rotationRate_y;
	double rotationRate_z;
	double userAcceleration_x;
	double userAcceleration_y;
	double userAcceleration_z;

	double total_acceleration_x = 1;
	double total_acceleration_y = 1;
	double total_acceleration_z = 1;

	std::string line;
	while (std::getline(current_file, line)) {
		std::istringstream iss(line);
		iss >>
			id >> delimiter >>
			attitude_roll >> delimiter >>
			attitude_pitch >> delimiter >>
			attitude_yaw >> delimiter >>
			gravity_x >> delimiter >>
			gravity_y >> delimiter >>
			gravity_z >> delimiter >>
			rotationRate_x >> delimiter >>
			rotationRate_y >> delimiter >>
			rotationRate_z >> delimiter >>
			userAcceleration_x >> delimiter >>
			userAcceleration_y >> delimiter >>
			userAcceleration_z >> delimiter;

		total_acceleration_x += userAcceleration_x;
		total_acceleration_y += userAcceleration_y;
		total_acceleration_z += userAcceleration_z;

		line_error |= last_id + 1 == id;
		last_id = id;
		total_lines++;
	}
}

int main() {
	const auto& data_filepath = fs::path{ BASE_FOLDER } / DATA_FOLDER;

	std::forward_list<fs::path> file_paths;
	get_files(data_filepath, file_paths);

	for (const fs::path file : file_paths) {
		proccess_file(file);
	}

	return EXIT_SUCCESS;
}