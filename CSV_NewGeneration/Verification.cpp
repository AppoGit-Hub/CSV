#include "Global.hpp"

[[nodiscard]] 
bool is_extreme(const double value, const double average, const double std) {
	const double z_score = (value - average) / std;
	return z_score > average + (3 * std) || z_score < average - (3 * std);
}

ProcessError verification() {
	std::fstream checkfile(CHECK_FILENAME, std::ios::out);
	if (!checkfile.is_open())
		return COUDLNT_OPEN_FILE;
	
	checkfile <<
		"Filename" << DELIMITER <<
		"Line" << DELIMITER <<
		"X" << DELIMITER <<
		"Y" << DELIMITER <<
		"Z" << DELIMITER << std::endl;

	for_file(DATA_FOLDERPATH, [&](const fs::path& file) {
		std::ifstream current_file(file);
		if (!current_file.is_open())
			return COUDLNT_OPEN_FILE;
		
		uint64_t total_extreme = 0;
		uint64_t total_normal = 0;

		std::string header;
		std::getline(current_file, header);

		std::string line;
		while (std::getline(current_file, line)) {
			total_normal++;

			std::istringstream iss(line);
				
			const RawLine line = RawLine::extract(iss);
				
			const bool extreme_x = is_extreme(line.user_acceleration_x, AVERAGE_X, STANDARD_DEVIATION_X);
			const bool extreme_y = is_extreme(line.user_acceleration_y, AVERAGE_Y, STANDARD_DEVIATION_Y);
			const bool extreme_z = is_extreme(line.user_acceleration_z, AVERAGE_Z, STANDARD_DEVIATION_Z);

			if (extreme_x || extreme_y || extreme_z) {
				total_extreme++;

				checkfile <<
					file << DELIMITER <<
					line.id - 1 << DELIMITER;

				if (extreme_x) {
					checkfile << line.user_acceleration_x << DELIMITER;
				}
				else {
					checkfile << DELIMITER;
				}

				if (extreme_y) {
					checkfile << line.user_acceleration_y << DELIMITER;
				}
				else {
					checkfile << DELIMITER;
				}

				if (extreme_z) {
					checkfile << line.user_acceleration_z << DELIMITER;
				}
				else {
					checkfile << DELIMITER;
				}

				checkfile << std::endl;
			}
		}

		std::cout << file.string() << " | " << total_normal << " | " << total_extreme << std::endl;
	});

	return NO_ERROR;
}