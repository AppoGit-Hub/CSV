#include "Global.hpp"

std::vector<double> find_acceleration(std::fstream& pattern, const MovementType movement_type) {
	std::vector<double> accelerations;

	pattern.clear();
	pattern.seekg(std::ios::beg);

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

	std::istringstream iss(line);

	iss >> movement >> delimiter;

	while (!iss.eof()) {
		iss >> acceleration >> delimiter;
		accelerations.push_back(acceleration);
	}
	
	return accelerations;
}

ProcessError evaluation(
	const std::string& testset_name, 
	const std::string& pattern_name
) {
	std::fstream testset(testset_name, std::ios::in);
	std::fstream pattern(pattern_name, std::ios::in);

	std::array<std::array<uint64_t, 6>, 6> verification = {0};
	std::unordered_map<MovementType, std::vector<double>> testset_lines;

	std::string header;
	std::getline(testset, header);

	char delimiter;
	uint64_t movement;
	uint64_t gender;
	uint64_t index;
	double testset_acceleration;

	std::string line;
	while (std::getline(testset, line)) {
		std::istringstream iss(line);

		iss >> movement >> delimiter 
			>> gender >> delimiter 
			>> index >> delimiter;

		std::streampos accleration_begin = iss.tellg();

		double distance_min = std::numeric_limits<double>::max();
		MovementType movement_min = MovementType::SIZE;

		std::vector<double> line_acceleration;
		uint64_t movement_size = static_cast<uint64_t>(MovementType::SIZE);
		for (uint64_t type_index = 1; type_index < movement_size; type_index++) {
			MovementType movement_type = static_cast<MovementType>(type_index);

			if (testset_lines.find(movement_type) != testset_lines.end()) {
				line_acceleration = testset_lines[movement_type];
			}
			else {
				line_acceleration = find_acceleration(pattern, movement_type);
				testset_lines[movement_type] = line_acceleration;
			}
			
			double total = 0;

			iss.clear();
			iss.seekg(accleration_begin, std::ios_base::beg);
			for (size_t line_index = 0; line_index < line_acceleration.size(); line_index++) {
				iss >> testset_acceleration >> delimiter;
				double pattern_acceleration = line_acceleration[line_index];

				total += std::pow(testset_acceleration - pattern_acceleration, 2);
			}

			double distance = std::sqrt(total);
			if (distance < distance_min) {
				distance_min = distance;
				movement_min = movement_type;
			}
		}
		
		MovementType type = static_cast<MovementType>(movement);
		uint64_t movement_id = static_cast<uint64_t>(movement_min);

		verification[movement - 1][movement_id - 1]++;
		//std::cout << movement << " >>> " << distance_min << " | " << static_cast<uint64_t>(movement_min) << std::endl;
	}

	std::fstream evaluation(EVALUATION_FILENAME, std::ios::out);
	evaluation <<
		"Mouvement" << DELIMITER <<
		"1" << DELIMITER <<
		"2" << DELIMITER <<
		"3" << DELIMITER <<
		"4" << DELIMITER <<
		"5" << DELIMITER <<
		"6" << DELIMITER << std::endl;

	uint64_t total_right = 0;
	uint64_t total = 0;
	for (size_t eval_index = 0; eval_index < verification.size(); eval_index++) {
		std::cout << (eval_index + 1) << " | ";
		evaluation << (eval_index + 1) << DELIMITER;
		
		uint64_t mov_total = 0;
		for (size_t guess_index = 0; guess_index < verification[eval_index].size(); guess_index++) {
			uint64_t guesses = verification[eval_index][guess_index];
			std::cout << guesses << " | ";
			evaluation << guesses << DELIMITER;
			mov_total += guesses;
			total += guesses;
		}
		evaluation << std::endl;

		uint64_t mov_right = verification[eval_index][eval_index];
		total_right += mov_right;

		double succes_rate = ((double)mov_right / mov_total) * 100;

		std::cout
			<< "Total : " << mov_total << " | "
			<< "Accuracy: " << succes_rate << "%" << std::endl;
	}

	std::cout
		<< "Total: "
		<< total_right << " | "
		<< total << " | "
		<< std::round(((double)total_right / total) * 100) << "%"
		<< std::endl;

	return NO_ERROR;
}

ProcessError phase_three() {
	return evaluation(TESTSET_FILENAME, PATTERN_FILENAME);
}