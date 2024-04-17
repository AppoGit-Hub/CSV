#include "Global.hpp"

ProcessError create_pattern(std::fstream& pattern, std::fstream& trainset) {
	pattern << "Mouvement";
	for (size_t index = 0; index < TRAINSET_COLUMNS; index++) {
		pattern << DELIMITER << "Vacc";
	}
	pattern << std::endl;

	std::string header;
	std::getline(trainset, header);

	char delimiter;

	uint64_t movement;
	uint64_t person_id;
	uint64_t gender;
	double acceleration;

	std::string line;
	std::string block_line;
	uint64_t block_movement;

	std::vector<double> accelerations;

	while (std::getline(trainset, line)) {
		std::istringstream iss(line);

		iss >> block_movement >> delimiter;
		movement = block_movement;

		accelerations.clear();
		size_t block_lines = 0;
		while (block_movement == movement && std::getline(trainset, block_line)) {
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

			std::cout << block_movement << " | " << cummulation << " | " << added << std::endl;
			block_lines++;
		}

		pattern << block_movement;
		for (size_t acc_index = 0; acc_index < accelerations.size(); acc_index++) {
			pattern << DELIMITER << accelerations[acc_index] / block_lines;
		}
		pattern << std::endl;
	}

	return NO_ERROR;
}

ProcessError phase_two() {
	std::fstream pattern(PATTERN_FILENAME, std::ios::out);
	if (!pattern.is_open())
		return COUDLNT_OPEN_FILE;

	std::fstream trainset(TRAINSET_FILENAME, std::ios::in);
	if (!trainset.is_open())
		return COUDLNT_OPEN_FILE;

	return create_pattern(pattern, trainset);
}