#include "Global.hpp"

static void process_file(const fs::path& current_path, const std::function<void(Line)> on_line) {
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

static bool is_extreme(const double value, const double average, const double std) {	
	double z_score = (value - average) / std;
	//std::cout << average - (3 * std) << " | " << z_score << " | " << average + (3 * std) << std::endl;
	return z_score > average + (3 * std) || z_score < average - (3 * std);
}

int main() {
	double sum_user_accelerations_x = 0;
	double sum_user_accelerations_y = 0;
	double sum_user_accelerations_z = 0;
	uint64_t total_lines = 0;

	get_files(DATA_FOLDERPATH, [&](const fs::path& file) {
		uint64_t valid_value_x = 0;
		uint64_t invalid_value_x = 0;
		uint64_t valid_value_y = 0;
		uint64_t invalid_value_y = 0;
		uint64_t valid_value_z = 0;
		uint64_t invalid_value_z = 0;
		process_file(file, [&](const Line& line) {
			if (is_extreme(line.gravity_x, AVERAGE_X, STANDARD_DEVIATION_X)) {
				//aberrations_acc_x.push_back({ number_line, line.gravity_x });
				//std::cout << line.gravity_x << std::endl;
				invalid_value_x++;
			}
			else {
				sum_user_accelerations_x += line.user_acceleration_x;
				valid_value_x++;
			}
			if (is_extreme(line.gravity_y, AVERAGE_Y, STANDARD_DEVIATION_Y)) {
				//aberrations_acc_y.push_back({ number_line, line.gravity_y });
				//std::cout << line.gravity_x << std::endl;
				invalid_value_y++;
			}
			else {
				sum_user_accelerations_y += line.user_acceleration_y;
				valid_value_y++;
			}
			if (is_extreme(line.gravity_z, AVERAGE_Z, STANDARD_DEVIATION_Z)) {
				//aberrations_acc_z.push_back({ number_line, line.gravity_z });
				//std::cout << line.gravity_x << std::endl;
				invalid_value_z++;
			}
			else {
				sum_user_accelerations_z += line.user_acceleration_z;
				valid_value_z++;
			}
			total_lines++;
		});
		std::cout << "X: " << file << " : " << valid_value_x << " | " << invalid_value_x << std::endl;
		std::cout << "Y: " << file << " : " << valid_value_y << " | " << invalid_value_y << std::endl;
		std::cout << "Z: " << file << " : " << valid_value_z << " | " << invalid_value_z << std::endl;
	});

	double average_acceleration_x = sum_user_accelerations_x / total_lines;
	double average_acceleration_y = sum_user_accelerations_y / total_lines;
	double average_acceleration_z = sum_user_accelerations_z / total_lines;

	double squared_deviations_x = 0;
	double squared_deviations_y = 0;
	double squared_deviations_z = 0;

	get_files(DATA_FOLDERPATH, [&](const fs::path& file) {
		process_file(file, [&](const Line& line) {
			squared_deviations_x += std::pow(line.user_acceleration_x - average_acceleration_x, 2);
			squared_deviations_y += std::pow(line.user_acceleration_y - average_acceleration_y, 2);
			squared_deviations_z += std::pow(line.user_acceleration_z - average_acceleration_z, 2);
		});	
	});

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