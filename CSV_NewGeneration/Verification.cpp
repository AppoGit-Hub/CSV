#include "global.hpp"

void verification(
	std::fstream& checkfile, 
	ExtremeFunction extreme_func
) {
	checkfile <<
		"Filename" << DELIMITER <<
		"Line" << DELIMITER <<
		"X" << DELIMITER <<
		"Y" << DELIMITER <<
		"Z" << DELIMITER << std::endl;

	double total_extreme = 0;
	double total_normal = 0;

	for_file(DATA_FOLDERPATH, [&](const fs::path& file) {
		std::ifstream current_file(file);
		if (!current_file.is_open())
			std::cerr << "Couldnt open: " << file << std::endl;

		std::string header;
		std::getline(current_file, header);

		std::string strline;
		while (std::getline(current_file, strline)) {
			total_normal++;

			RawLine rawline;
			std::istringstream iss(strline);
			extract_rawline(rawline, iss);

			const bool extreme_x = extreme_func(rawline.user_acceleration_x, AVERAGE_X, STANDARD_DEVIATION_X);
			const bool extreme_y = extreme_func(rawline.user_acceleration_y, AVERAGE_Y, STANDARD_DEVIATION_Y);
			const bool extreme_z = extreme_func(rawline.user_acceleration_z, AVERAGE_Z, STANDARD_DEVIATION_Z);

			if (extreme_x || extreme_y || extreme_z) {
				total_extreme++;

				checkfile <<
					file << DELIMITER <<
					rawline.id - 1 << DELIMITER;

				if (extreme_x) {
					checkfile << rawline.user_acceleration_x << DELIMITER;
				}
				else {
					checkfile << DELIMITER;
				}

				if (extreme_y) {
					checkfile << rawline.user_acceleration_y << DELIMITER;
				}
				else {
					checkfile << DELIMITER;
				}

				if (extreme_z) {
					checkfile << rawline.user_acceleration_z << DELIMITER;
				}
				else {
					checkfile << DELIMITER;
				}

				checkfile << std::endl;
			}
		}
	});
}

void phase_one() {
	std::fstream checkfile(CHECK_FILENAME, std::ios::out);
	if (!checkfile.is_open())
		std::cerr << "Couldnt open: " << CHECK_FILENAME << std::endl;
	
	verification(checkfile, is_extreme);
}