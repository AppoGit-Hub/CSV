#include "Global.hpp"

std::string find_acceleration_xyz(
	const std::string& pattern_name,
	uint64_t movement_type
) {
	std::ifstream pattern(pattern_name);
		
	std::string header;
	std::getline(pattern, header);

	char delimiter;
	uint64_t movement = 0;
	double acceleration;

	uint64_t movement_search = static_cast<uint64_t>(movement_type);

	std::string line;
	while (movement != movement_search && std::getline(pattern, line)) {
		std::istringstream iss(line);
		iss >> movement >> delimiter;
	}
	
	return line;
}

void evaluation_xyz() {
	std::unordered_map<MovementType, EvalutionStats> verification;

	std::ifstream testset_attitude_roll("testset_attitude_roll.csv");
	std::ifstream testset_attitude_pitch("testset_attitude_pitch.csv");
	std::ifstream testset_attitude_yaw("testset_attitude_yaw.csv");
	std::ifstream testset_gravity_x("testset_gravity_x.csv");
	std::ifstream testset_gravity_y("testset_gravity_y.csv");
	std::ifstream testset_gravity_z("testset_gravity_z.csv");
	std::ifstream testset_rotation_rate_x("testset_rotation_rate_x.csv");
	std::ifstream testset_rotation_rate_y("testset_rotation_rate_y.csv");
	std::ifstream testset_rotation_rate_z("testset_rotation_rate_z.csv");
	std::ifstream testset_user_acceleration_x("testset_user_acceleration_x.csv");
	std::ifstream testset_user_acceleration_y("testset_user_acceleration_y.csv");
	std::ifstream testset_user_acceleration_z("testset_user_acceleration_z.csv");

	std::string header_attitude_roll;
	std::string header_attitude_pitch;
	std::string header_attitude_yaw;
	std::string header_gravity_x;
	std::string header_gravity_y;
	std::string header_gravity_z;
	std::string header_rotation_rate_x;
	std::string header_rotation_rate_y;
	std::string header_rotation_rate_z;
	std::string header_user_acceleration_x;
	std::string header_user_acceleration_y;
	std::string header_user_acceleration_z;

	std::getline(testset_attitude_roll, header_attitude_roll);
	std::getline(testset_attitude_pitch, header_attitude_pitch);
	std::getline(testset_attitude_yaw, header_attitude_yaw);
	std::getline(testset_gravity_x, header_gravity_x);
	std::getline(testset_gravity_y, header_gravity_y);
	std::getline(testset_gravity_z, header_gravity_z);
	std::getline(testset_rotation_rate_x, header_rotation_rate_x);
	std::getline(testset_rotation_rate_y, header_rotation_rate_y);
	std::getline(testset_rotation_rate_z, header_rotation_rate_z);
	std::getline(testset_user_acceleration_x, header_user_acceleration_x);
	std::getline(testset_user_acceleration_y, header_user_acceleration_y);
	std::getline(testset_user_acceleration_z, header_user_acceleration_z);

	size_t line_index = 0;
	while (line_index < 360) {
		std::string testset_attitude_roll_line;
		std::string testset_attitude_pitch_line;
		std::string testset_attitude_yaw_line;
		std::string testset_gravity_x_line;
		std::string testset_gravity_y_line;
		std::string testset_gravity_z_line;
		std::string testset_rotation_rate_x_line;
		std::string testset_rotation_rate_y_line;
		std::string testset_rotation_rate_z_line;
		std::string testset_user_acceleration_x_line;
		std::string testset_user_acceleration_y_line;
		std::string testset_user_acceleration_z_line;

		std::getline(testset_attitude_roll, testset_attitude_roll_line);
		std::getline(testset_attitude_pitch, testset_attitude_pitch_line);
		std::getline(testset_attitude_yaw, testset_attitude_yaw_line);
		std::getline(testset_gravity_x, testset_gravity_x_line);
		std::getline(testset_gravity_y, testset_gravity_y_line);
		std::getline(testset_gravity_z, testset_gravity_z_line);
		std::getline(testset_rotation_rate_x, testset_rotation_rate_x_line);
		std::getline(testset_rotation_rate_y, testset_rotation_rate_y_line);
		std::getline(testset_rotation_rate_z, testset_rotation_rate_z_line);
		std::getline(testset_user_acceleration_x, testset_user_acceleration_x_line);
		std::getline(testset_user_acceleration_y, testset_user_acceleration_y_line);
		std::getline(testset_user_acceleration_z, testset_user_acceleration_z_line);

		std::istringstream testset_attitude_roll_ss(testset_attitude_roll_line);
		std::istringstream testset_attitude_pitch_ss(testset_attitude_pitch_line);
		std::istringstream testset_attitude_yaw_ss(testset_attitude_yaw_line);
		std::istringstream testset_gravity_x_ss(testset_gravity_x_line);
		std::istringstream testset_gravity_y_ss(testset_gravity_y_line);
		std::istringstream testset_gravity_z_ss(testset_gravity_z_line);
		std::istringstream testset_rotation_rate_x_ss(testset_rotation_rate_x_line);
		std::istringstream testset_rotation_rate_y_ss(testset_rotation_rate_y_line);
		std::istringstream testset_rotation_rate_z_ss(testset_rotation_rate_z_line);
		std::istringstream testset_user_acceleration_x_ss(testset_user_acceleration_x_line);
		std::istringstream testset_user_acceleration_y_ss(testset_user_acceleration_y_line);
		std::istringstream testset_user_acceleration_z_ss(testset_user_acceleration_z_line);

		char delimiter;

		uint64_t testset_movement;
		uint64_t testset_gender;
		uint64_t testset_index;

		testset_attitude_roll_ss >> testset_movement >> delimiter >> testset_gender >> delimiter >> testset_index >> delimiter;
		testset_attitude_pitch_ss >> testset_movement >> delimiter >> testset_gender >> delimiter >> testset_index >> delimiter;
		testset_attitude_yaw_ss >> testset_movement >> delimiter >> testset_gender >> delimiter >> testset_index >> delimiter;
		testset_gravity_x_ss >> testset_movement >> delimiter >> testset_gender >> delimiter >> testset_index >> delimiter;
		testset_gravity_y_ss >> testset_movement >> delimiter >> testset_gender >> delimiter >> testset_index >> delimiter;
		testset_gravity_z_ss >> testset_movement >> delimiter >> testset_gender >> delimiter >> testset_index >> delimiter;
		testset_rotation_rate_x_ss >> testset_movement >> delimiter >> testset_gender >> delimiter >> testset_index >> delimiter;
		testset_rotation_rate_y_ss >> testset_movement >> delimiter >> testset_gender >> delimiter >> testset_index >> delimiter;
		testset_rotation_rate_z_ss >> testset_movement >> delimiter >> testset_gender >> delimiter >> testset_index >> delimiter;
		testset_user_acceleration_x_ss >> testset_movement >> delimiter >> testset_gender >> delimiter >> testset_index >> delimiter;
		testset_user_acceleration_y_ss >> testset_movement >> delimiter >> testset_gender >> delimiter >> testset_index >> delimiter;
		testset_user_acceleration_z_ss >> testset_movement >> delimiter >> testset_gender >> delimiter >> testset_index >> delimiter;

		double distance_min = std::numeric_limits<double>::max();
		uint64_t movement_min = 7;

		for (size_t movement_index = 1; movement_index < 7; movement_index++) {
			std::string pattern_attitude_roll_line = find_acceleration_xyz("pattern_attitude_roll.csv", movement_index);
			std::string pattern_attitude_pitch_line = find_acceleration_xyz("pattern_attitude_pitch.csv", movement_index);
			std::string pattern_attitude_yaw_line = find_acceleration_xyz("pattern_attitude_yaw.csv", movement_index);
			std::string pattern_gravity_x_line = find_acceleration_xyz("pattern_gravity_x.csv", movement_index);
			std::string pattern_gravity_y_line = find_acceleration_xyz("pattern_gravity_y.csv", movement_index);
			std::string pattern_gravity_z_line = find_acceleration_xyz("pattern_gravity_z.csv", movement_index);
			std::string pattern_rotation_rate_x_line = find_acceleration_xyz("pattern_rotation_rate_x.csv", movement_index);
			std::string pattern_rotation_rate_y_line = find_acceleration_xyz("pattern_rotation_rate_y.csv", movement_index);
			std::string pattern_rotation_rate_z_line = find_acceleration_xyz("pattern_rotation_rate_z.csv", movement_index);
			std::string pattern_user_acceleration_x_line = find_acceleration_xyz("pattern_user_acceleration_x.csv", movement_index);
			std::string pattern_user_acceleration_y_line = find_acceleration_xyz("pattern_user_acceleration_y.csv", movement_index);
			std::string pattern_user_acceleration_z_line = find_acceleration_xyz("pattern_user_acceleration_z.csv", movement_index);

			std::istringstream pattern_attitude_roll_ss(pattern_attitude_roll_line);
			std::istringstream pattern_attitude_pitch_ss(pattern_attitude_pitch_line);
			std::istringstream pattern_attitude_yaw_ss(pattern_attitude_yaw_line);
			std::istringstream pattern_gravity_x_ss(pattern_gravity_x_line);
			std::istringstream pattern_gravity_y_ss(pattern_gravity_y_line);
			std::istringstream pattern_gravity_z_ss(pattern_gravity_z_line);
			std::istringstream pattern_rotation_rate_x_ss(pattern_rotation_rate_x_line);
			std::istringstream pattern_rotation_rate_y_ss(pattern_rotation_rate_y_line);
			std::istringstream pattern_rotation_rate_z_ss(pattern_rotation_rate_z_line);
			std::istringstream pattern_user_acceleration_x_ss(pattern_user_acceleration_x_line);
			std::istringstream pattern_user_acceleration_y_ss(pattern_user_acceleration_y_line);
			std::istringstream pattern_user_acceleration_z_ss(pattern_user_acceleration_z_line);

			uint64_t pattern_movement;

			pattern_attitude_roll_ss >> pattern_movement >> delimiter;
			pattern_attitude_pitch_ss >> pattern_movement >> delimiter;
			pattern_attitude_yaw_ss >> pattern_movement >> delimiter;
			pattern_gravity_x_ss >> pattern_movement >> delimiter;
			pattern_gravity_y_ss >> pattern_movement >> delimiter;
			pattern_gravity_z_ss >> pattern_movement >> delimiter;
			pattern_rotation_rate_x_ss >> pattern_movement >> delimiter;
			pattern_rotation_rate_y_ss >> pattern_movement >> delimiter;
			pattern_rotation_rate_z_ss >> pattern_movement >> delimiter;
			pattern_user_acceleration_x_ss >> pattern_movement >> delimiter;
			pattern_user_acceleration_y_ss >> pattern_movement >> delimiter;
			pattern_user_acceleration_z_ss >> pattern_movement >> delimiter;

			double testset_attitude_roll;
			double testset_attitude_pitch;
			double testset_attitude_yaw;
			double testset_gravity_x;
			double testset_gravity_y;
			double testset_gravity_z;
			double testset_rotation_rate_x;
			double testset_rotation_rate_y;
			double testset_rotation_rate_z;
			double testset_user_acceleration_x;
			double testset_user_acceleration_y;
			double testset_user_acceleration_z;

			double pattern_attitude_roll;
			double pattern_attitude_pitch;
			double pattern_attitude_yaw;
			double pattern_gravity_x;
			double pattern_gravity_y;
			double pattern_gravity_z;
			double pattern_rotation_rate_x;
			double pattern_rotation_rate_y;
			double pattern_rotation_rate_z;
			double pattern_user_acceleration_x;
			double pattern_user_acceleration_y;
			double pattern_user_acceleration_z;

			double total_attitude_roll = 0;
			double total_attitude_pitch = 0;
			double total_attitude_yaw = 0;
			double total_gravity_x = 0;
			double total_gravity_y = 0;
			double total_gravity_z = 0;
			double total_rotation_rate_x = 0;
			double total_rotation_rate_y = 0;
			double total_rotation_rate_z = 0;
			double total_user_acceleration_x = 0;
			double total_user_acceleration_y = 0;
			double total_user_acceleration_z = 0;

			// hope len(pattern_ss_x) == len(pattern_ss_y) == len(pattern_ss_z)

			while (!pattern_attitude_roll_ss.eof()) {
				testset_attitude_roll_ss >> testset_attitude_roll >> delimiter;
				testset_attitude_pitch_ss >> testset_attitude_pitch >> delimiter;
				testset_attitude_yaw_ss >> testset_attitude_yaw >> delimiter;
				testset_gravity_x_ss >> testset_gravity_x >> delimiter;
				testset_gravity_y_ss >> testset_gravity_y >> delimiter;
				testset_gravity_z_ss >> testset_gravity_z >> delimiter;
				testset_rotation_rate_x_ss >> testset_rotation_rate_x >> delimiter;
				testset_rotation_rate_y_ss >> testset_rotation_rate_y >> delimiter;
				testset_rotation_rate_z_ss >> testset_rotation_rate_z >> delimiter;
				testset_user_acceleration_x_ss >> testset_user_acceleration_x >> delimiter;
				testset_user_acceleration_y_ss >> testset_user_acceleration_y >> delimiter;
				testset_user_acceleration_z_ss >> testset_user_acceleration_z >> delimiter;

				pattern_attitude_roll_ss >> pattern_attitude_roll >> delimiter;
				pattern_attitude_pitch_ss >> pattern_attitude_pitch >> delimiter;
				pattern_attitude_yaw_ss >> pattern_attitude_yaw >> delimiter;
				pattern_gravity_x_ss >> pattern_gravity_x >> delimiter;
				pattern_gravity_y_ss >> pattern_gravity_y >> delimiter;
				pattern_gravity_z_ss >> pattern_gravity_z >> delimiter;
				pattern_rotation_rate_x_ss >> pattern_rotation_rate_x >> delimiter;
				pattern_rotation_rate_y_ss >> pattern_rotation_rate_y >> delimiter;
				pattern_rotation_rate_z_ss >> pattern_rotation_rate_z >> delimiter;
				pattern_user_acceleration_x_ss >> pattern_user_acceleration_x >> delimiter;
				pattern_user_acceleration_y_ss >> pattern_user_acceleration_y >> delimiter;
				pattern_user_acceleration_z_ss >> pattern_user_acceleration_z >> delimiter;

				double testset_lenght = std::sqrt
				(
					std::pow(testset_attitude_roll, 2) +
					std::pow(testset_attitude_pitch, 2) +
					std::pow(testset_attitude_yaw, 2) +
					std::pow(testset_gravity_x, 2) +
					std::pow(testset_gravity_y, 2) +
					std::pow(testset_gravity_z, 2) +
					std::pow(testset_rotation_rate_x, 2) +
					std::pow(testset_rotation_rate_y, 2) +
					std::pow(testset_rotation_rate_z, 2) +
					std::pow(testset_user_acceleration_x, 2) +
					std::pow(testset_user_acceleration_y, 2) +
					std::pow(testset_user_acceleration_z, 2)
				);

				double pattern_lenght = std::sqrt
				(
					std::pow(pattern_attitude_roll, 2) + 
					std::pow(pattern_attitude_pitch, 2) +
					std::pow(pattern_attitude_yaw, 2) +
					std::pow(pattern_gravity_x, 2) +
					std::pow(pattern_gravity_y, 2) +
					std::pow(pattern_gravity_z, 2) +
					std::pow(pattern_rotation_rate_x, 2) +
					std::pow(pattern_rotation_rate_y, 2) +
					std::pow(pattern_rotation_rate_z, 2) +
					std::pow(pattern_user_acceleration_x, 2) +
					std::pow(pattern_user_acceleration_y, 2) +
					std::pow(pattern_user_acceleration_z, 2)
				);

				double testset_pattern_product =
					(testset_attitude_roll * pattern_attitude_roll) +
					(testset_attitude_pitch * pattern_attitude_pitch) +
					(testset_attitude_yaw * pattern_attitude_yaw) +
					(testset_gravity_x * pattern_gravity_x) +
					(testset_gravity_y * pattern_gravity_y) +
					(testset_gravity_z * pattern_gravity_z) +
					(testset_rotation_rate_x * pattern_rotation_rate_x) +
					(testset_rotation_rate_y * pattern_rotation_rate_y) +
					(testset_rotation_rate_z * pattern_rotation_rate_z) +
					(testset_user_acceleration_x * pattern_user_acceleration_x) +
					(testset_user_acceleration_y * pattern_user_acceleration_y) +
					(testset_user_acceleration_z * pattern_user_acceleration_z);

				double cosinus_angle = testset_pattern_product / (testset_lenght * pattern_lenght);
			
				if (cosinus_angle < distance_min) {
					distance_min = cosinus_angle;
					movement_min = movement_index;
				}
			}
		}

		std::cout
			<< "Movement: " << testset_movement << " | "
			<< "Movement Min: " << movement_min << " | "
			<< "Distance Min: " << distance_min << " | "
			<< std::endl;

		MovementType type = static_cast<MovementType>(testset_movement);
		if (verification.find(type) == verification.end()) {
			verification[type] = EvalutionStats{};
		}

		EvalutionStats& evalution = verification[type];
		uint64_t movement_id = static_cast<uint64_t>(movement_min);

		evalution.guess_at[movement_id - 1]++;

		line_index++;
	}

	for (auto [type, evaluation] : verification) {
		uint64_t type_id = static_cast<uint64_t>(type);
		std::cout << type_id << " | ";

		uint64_t total = 0;
		for (uint64_t guesses : evaluation.guess_at) {
			std::cout << guesses << " | ";
			total += guesses;
		}

		double succes_rate = ((double)evaluation.guess_at[type_id - 1] / total) * 100;

		std::cout
			<< "Total : " << total << " | "
			<< "Accuracy: " << succes_rate << "%" << std::endl;
	}
}