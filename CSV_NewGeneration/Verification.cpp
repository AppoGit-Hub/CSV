#include "Global.hpp"

[[nodiscard]] const bool is_extreme(const double value, const double average, const double std) {
	return value > average + (3 * std) || value < average - (3 * std);
}

void verification() {
	std::ofstream output_file(CHECK_FILENAME);
	if (!output_file.is_open()) {
		std::cout << "Couldnt open file : " << CHECK_FILENAME;
	}
	else {
		output_file <<
			"Filename" << DELIMITER <<
			"Line" << DELIMITER <<
			"X" << DELIMITER <<
			"Y" << DELIMITER <<
			"Z" << DELIMITER << std::endl;

		for_file(DATA_FOLDERPATH, [&](const fs::path& file) {
			std::ifstream current_file(file);

			std::string header;
			std::getline(current_file, header);

			std::string line;
			while (std::getline(current_file, line)) {
				std::istringstream iss(line);
				
				const RawLine line = RawLine::extract(iss);
				
				const bool extreme_x = is_extreme(line.user_acceleration_x, AVERAGE_X, STANDARD_DEVIATION_X);
				const bool extreme_y = is_extreme(line.user_acceleration_y, AVERAGE_Y, STANDARD_DEVIATION_Y);
				const bool extreme_z = is_extreme(line.user_acceleration_z, AVERAGE_Z, STANDARD_DEVIATION_Z);

				if (extreme_x || extreme_y || extreme_z) {
					output_file <<
						file << DELIMITER <<
						line.id - 1 << DELIMITER;

					if (extreme_x) {
						output_file << line.user_acceleration_x << DELIMITER;
					}
					else {
						output_file << DELIMITER;
					}

					if (extreme_y) {
						output_file << line.user_acceleration_y << DELIMITER;
					}
					else {
						output_file << DELIMITER;
					}

					if (extreme_z) {
						output_file << line.user_acceleration_z << DELIMITER;
					}
					else {
						output_file << DELIMITER;
					}

					output_file << std::endl;
				}
			}
			current_file.close();
		});
	}
}