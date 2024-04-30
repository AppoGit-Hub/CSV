#include "Global.hpp"

ProcessError create_pattern(
	const std::string& pattern_name, 
	const std::string& trainset_name
) {
	std::ofstream pattern(pattern_name);
	std::ifstream trainset(trainset_name);
	
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

	std::vector<std::pair<double, uint64_t>> accelerations;

	while (std::getline(trainset, line)) {
		std::istringstream iss(line);

		iss >> block_movement >> delimiter;
		movement = block_movement;

		accelerations.clear();

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
					accelerations[acc_index].first += acceleration;
					accelerations[acc_index].second++;
					cummulation++;
				}
				else {
					accelerations.push_back(std::pair(acceleration, 1));
					added++;
				}
			}

			std::cout << block_movement << " | " << cummulation << " | " << added << std::endl;
		}

		pattern << block_movement;
		for (size_t acc_index = 0; acc_index < accelerations.size(); acc_index++) {
			pattern << DELIMITER << accelerations[acc_index].first / accelerations[acc_index].second;
		}
		pattern << std::endl;
	}

	return NO_ERROR;
}

ProcessError phase_two() {
	return create_pattern(PATTERN_FILENAME, TRAINSET_FILENAME);
}