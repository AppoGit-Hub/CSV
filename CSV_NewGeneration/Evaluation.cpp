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

ProcessError evaluation(std::fstream& testset, std::fstream& pattern) {
	std::unordered_map<MovementType, EvalutionStats> verification;
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
		if (verification.find(type) == verification.end()) {
			verification[type] = EvalutionStats{};
		}
		
		EvalutionStats& evalution = verification[type];
		uint64_t movement_id = static_cast<uint64_t>(movement_min);

		evalution.guess_at[movement_id - 1]++;

		//std::cout << movement << " >>> " << distance_min << " | " << static_cast<uint64_t>(movement_min) << std::endl;
	}

	//std::cout << "Verification: " << std::endl;
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
	/*
	std::cout 
		<< "Total: " 
		<< right_global << " | " 
		<< total_global << " | "
		<< std::round((right_global / total_global) * 100) << "%" 
		<< std::endl;
	*/
	return NO_ERROR;
}

ProcessError phase_three() {
	std::fstream testset(TESTSET_FILENAME, std::ios::in);
	if (!testset.is_open())
		return COUDLNT_OPEN_FILE;

	std::fstream pattern(PATTERN_FILENAME, std::ios::in);
	if (!pattern.is_open())
		return COUDLNT_OPEN_FILE;

	return evaluation(testset, pattern);
}