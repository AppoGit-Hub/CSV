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
inline const std::string DATA_CHECK = "résultats vérification";

inline const double AVERAGE_X = 0.00096087;
inline const double AVERAGE_Y = 0.05525659;
inline const double AVERAGE_Z = 0.0352192;
inline const double STANDARD_DEVIATION_X = 0.38875666;
inline const double STANDARD_DEVIATION_Y = 0.61937128;
inline const double STANDARD_DEVIATION_Z = 0.4300345;

struct Aberration {
	uint64_t index;
	double value; 
};

static void process_file(const fs::path& current_path, const std::function<void(const Line&)> on_line) {
	std::ifstream current_file(current_path);
	
	std::string header;
	std::getline(current_file, header);

	char delimiter;
	Line current_line;
	uint64_t number_line = 1;

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

		on_line(current_line, number_line);
		number_line++; 
	}
}

static void check_data() {
	std::ofstream file_check(DATA_CHECK);
	if (!file_check.is_open()) {
		sdt::cout << "ouverture du fichier (" << DATA_CHECK << ") : échouer"<<std::endl;
	}
	else {
		const auto& data_filepath = fs::path{ BASE_FOLDER } / DATA_FOLDER;

		std::forward_list<fs::path> file_paths;
		get_files(data_filepath, file_paths);

		for (const fs::path file : file_paths) {
			std::string file_name = file.filename();

			uint64_t nb_aberrations_acc_x = 0;
			uint64_t nb_aberrations_acc_y = 0;
			uint64_t nb_aberrations_acc_z = 0;

			std::vector<Aberration> aberrations_acc_x;
			std::vector<Aberration> aberrations_acc_y;
			std::vector<Aberration> aberrations_acc_z;

			uint64_t nb_lines = 0;
			process_file(file, [&](const Line& line, uint64_t number_line) {
				if (line.gravity_x > AVERAGE_X + 3 * STANDARD_DEVIATION_X || line.gravity_x < AVERAGE_X - 3 * STANDARD_DEVIATION_X) {
					aberration_acc_x.push_back({ number_line,line.gravity_x });
					nb_aberrations_acc_x++;
				}
				if (line.gravity_y > AVERAGE_Y + 3 * STANDARD_DEVIATION_Y || line.gravity_y < AVERAGE_Y - 3 * STANDARD_DEVIATION_Y) {
					aberration_acc_y.push_back({ number_line,line.gravity_y });
					nb_aberrations_acc_y++;
				}
				if (line.gravity_z > AVERAGE_Z + 3 * STANDARD_DEVIATION_Z || line.gravity_z < AVERAGE_Z - 3 * STANDARD_DEVIATION_Z) {
					aberration_acc_z.push_back({ number_line,line.gravity_z });
					nb_aberrations_acc_z++;
				}
				nb_lines++;
			});

			file_check << file_name << DELIMITER
				<< nb_lines << std:enl;
			file_check << nb_aberrations_acc_x;
			for (Aberration aberration : nb_aberrations_acc_x) {
				file_check << aberration.index << DELIMITER << aberration.value << DELIMITER;
			}
			file_check << std::enl;

			file_check << nb_aberrations_acc_y;
			for (Aberration aberration : nb_aberrations_acc_y) {
				file_check << aberration.index << DELIMITER << aberration.value << DELIMITER;
			}
			file_check << std::enl;

			file_check << nb_aberrations_acc_z;
			for (Aberration aberration : nb_aberrations_acc_z) {
				file_check << aberration.index << DELIMITER << aberration.value << DELIMITER;
			}
			file_check << std::enl;
		}
		file_check.close();
	}
}

int main() {
	const auto& data_filepath = fs::path{BASE_FOLDER}/DATA_FOLDER;

	std::forward_list<fs::path> file_paths;
	get_files(data_filepath, file_paths);

	for (const fs::path file : file_paths) {
		double sum_user_accelerations_x = 0;
		double sum_user_accelerations_y = 0;
		double sum_user_accelerations_z = 0;
		uint64_t total_lines = 0;

		process_file(file, [&](const Line& line) {
			sum_user_accelerations_x += line.user_acceleration_x;
			sum_user_accelerations_y += line.user_acceleration_y;
			sum_user_accelerations_z += line.user_acceleration_z;
			total_lines++;
		});
	
		double average_acceleration_x = sum_user_accelerations_x / total_lines;
		double average_acceleration_y = sum_user_accelerations_y / total_lines;
		double average_acceleration_z = sum_user_accelerations_z / total_lines;
		
		double squared_deviations_x = 0;
		double squared_deviations_y = 0;
		double squared_deviations_z = 0;

		proccess_file(file, [&](const Line& line) {
			squared_deviations_x += std::pow(line.user_acceleration_x - average_acceleration_x, 2);
			squared_deviations_y += std::pow(line.user_acceleration_y - average_acceleration_y, 2);
			squared_deviations_z += std::pow(line.user_acceleration_z - average_acceleration_z, 2);
		});

		double std_deviation_x = std::sqrt(squared_deviations_x / total_lines);
		double std_deviation_y = std::sqrt(squared_deviations_y / total_lines);
		double std_deviation_z = std::sqrt(squared_deviations_z / total_lines);
	
		std::cout << "Standard Deviation X: " << std_deviation_x << std::endl;
		std::cout << "Standard Deviation Y: " << std_deviation_y << std::endl;
		std::cout << "Standard Deviation Z: " << std_deviation_z << std::endl;
	}

	return EXIT_SUCCESS;
}