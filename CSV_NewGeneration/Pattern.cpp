#include "Global.hpp"

void pattern() {
	std::ofstream pattern_file(PATTERN_FILENAME);
	pattern_file << "Mouvement";
	for (size_t index = 0; index < TRAINSET_COLUMNS; index++) {
		pattern_file << DELIMITER << "Vacc";
	}
	pattern_file << std::endl;

	std::ifstream current_file(TRAINSET_FILENAME);

	std::string header;
	std::getline(current_file, header);

	char delimiter;

	uint64_t movement;
	uint64_t person_id;
	uint64_t gender;
	double acceleration;

	std::string line;
	std::string block_line;
	uint64_t block_movement;

	std::vector<double> accelerations;

	while (std::getline(current_file, line)) {
		std::istringstream iss(line);

		iss >> block_movement >> delimiter;
		movement = block_movement;

		accelerations.clear();
		while (block_movement == movement && std::getline(current_file, block_line)) {
			std::istringstream block_iss(block_line);

			block_iss >>
				movement >> delimiter >>
				person_id >> delimiter >>
				gender >> delimiter;

			size_t added = 0;
			size_t cummulation = 0;

			for (size_t acc_index = 0; !block_iss.eof(); acc_index++) {
				block_iss >> acceleration >> delimiter;

				if (acc_index < accelerations.size()) {
					accelerations[acc_index] += acceleration;
					cummulation++;
				}
				else {
					accelerations.push_back(acceleration);
					added++;
				}
			}

			size_t size = accelerations.size();
			for (size_t acc_index = 0; acc_index < size; acc_index++) {
				accelerations[acc_index] /= size;
			}

			std::cout << block_movement << " | " << cummulation << " | " << added << std::endl;		
		}

		pattern_file << block_movement;
		for (size_t acc_index = 0; acc_index < accelerations.size(); acc_index++) {
			pattern_file << DELIMITER << accelerations[acc_index];
		}
		pattern_file << std::endl;
	}

	pattern_file.close();
	current_file.close();
}