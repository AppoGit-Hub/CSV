#include "Global.hpp"

struct Line {
	uint64_t id;
	double attitude_roll;
	double attitude_pitch;
	double attitude_yaw;
	double gravity_x;
	double gravity_y;
	double gravity_z;
	double rotation_rate_x;
	double rotation_rate_y;
	double rotation_rate_z;
	double user_acceleration_x;
	double user_acceleration_y;
	double user_acceleration_z;
};

inline const std::string DELIMITER = ",";
inline const std::string BASE_FOLDER = "archive";
inline const std::string DATA_FOLDER = "data";

static void proccess_file(const fs::path& current_path, const std::function<void(const Line&)> on_line) {
	std::ifstream current_file(current_path);
	
	std::string header;
	std::getline(current_file, header);

	char delimiter;
	Line current_line;

	std::string line;
	while (std::getline(current_file, line)) {
		std::istringstream iss(line);
		iss >>
			current_line.id >> delimiter >>
			current_line.attitude_roll >> delimiter >>
			current_line.attitude_pitch >> delimiter >>
			current_line.attitude_yaw >> delimiter >>
			current_line.gravity_x >> delimiter >>
			current_line.gravity_y >> delimiter >>
			current_line.gravity_z >> delimiter >>
			current_line.rotation_rate_x >> delimiter >>
			current_line.rotation_rate_y >> delimiter >>
			current_line.rotation_rate_z >> delimiter >>
			current_line.user_acceleration_x >> delimiter >>
			current_line.user_acceleration_y >> delimiter >>
			current_line.user_acceleration_z >> delimiter;

		on_line(current_line);
	}

	current_file.close();
}

int main() {
	const auto& data_filepath = fs::path{BASE_FOLDER}/DATA_FOLDER;

	std::forward_list<fs::path> file_paths;
	get_files(data_filepath, file_paths);

	double user_acceleration_x = 0;
	double user_acceleration_y = 0;
	double user_acceleration_z = 0;
	uint64_t total_lines = 0;

	for (const fs::path file : file_paths) {
		proccess_file(file, [&](const Line& line) {
			user_acceleration_x += line.user_acceleration_x;
			user_acceleration_y += line.user_acceleration_y;
			user_acceleration_z += line.user_acceleration_z;
			total_lines++;
		});
	}

	double average_acceleration_x = user_acceleration_x / total_lines;
	double average_acceleration_y = user_acceleration_y / total_lines;
	double average_acceleration_z = user_acceleration_z / total_lines;

	double squared_deviations_x = 0;
	double squared_deviations_y = 0;
	double squared_deviations_z = 0;

	for (const fs::path file : file_paths) {
		proccess_file(file, [&](const Line& line) {
			squared_deviations_x += std::pow(line.user_acceleration_x - average_acceleration_x, 2);
			squared_deviations_y += std::pow(line.user_acceleration_y - average_acceleration_y, 2);
			squared_deviations_z += std::pow(line.user_acceleration_z - average_acceleration_z, 2);
		});
	}

	double std_deviation_x = std::sqrt(squared_deviations_x / total_lines);
	double std_deviation_y = std::sqrt(squared_deviations_y / total_lines);
	double std_deviation_z = std::sqrt(squared_deviations_z / total_lines);

	std::cout << "Average X: " << average_acceleration_x << std::endl;
	std::cout << "Average Y: " << average_acceleration_y << std::endl;
	std::cout << "Average Z: " << average_acceleration_z << std::endl;

	std::cout << "Standard Deviation X: " << std_deviation_x << std::endl;
	std::cout << "Standard Deviation Y: " << std_deviation_y << std::endl;
	std::cout << "Standard Deviation Z: " << std_deviation_z << std::endl;

	return EXIT_SUCCESS;
}