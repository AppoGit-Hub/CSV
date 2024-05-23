#include "global.hpp"

void create_pattern(
	std::vector<std::vector<double>>& trainset_matrix,
	std::vector<std::vector<double>>& pattern_matrix,
	std::fstream& pattern
) {
	size_t trainset_row_index = 0;
	while (trainset_row_index < trainset_matrix.size()) {
		auto row = trainset_matrix[trainset_row_index];
		auto movement = trainset_matrix[trainset_row_index][0];

		std::vector<std::pair<double, uint64_t>> accelerations;

		while (trainset_row_index < trainset_matrix.size() && movement == trainset_matrix[trainset_row_index][0]) {
			for (size_t column_index = 1; column_index < row.size(); column_index++) {
				double acceleretion = trainset_matrix[trainset_row_index][column_index];
				size_t acceleration_index = column_index - 1;

				if (acceleration_index < accelerations.size()) {
					accelerations[acceleration_index].first += acceleretion;
					accelerations[acceleration_index].second++;
				}
				else {
					accelerations.push_back(std::pair(acceleretion, 1));
				}
			}
			trainset_row_index++;
		}

#if LOGFILE
		pattern << movement;
#endif
		pattern_matrix[movement - 1][0] = (double)(movement);
		for (size_t acc_index = 0; acc_index < accelerations.size(); acc_index++) {
			double cummulation = accelerations[acc_index].first;
			uint64_t stack_count = accelerations[acc_index].second;

			double value = cummulation / stack_count;
#if LOGFILE
			pattern << DELIMITER << value;
#endif
			pattern_matrix[movement - 1][acc_index + 1] = (double)(value);
		}
#if LOGFILE
		pattern << std::endl;
#endif
	}
}