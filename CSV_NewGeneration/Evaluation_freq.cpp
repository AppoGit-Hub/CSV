#include "Global.hpp"

struct PatternEvaluation {
	double min;
	double max;
};


void evaluation_freq(const std::string& testset_name, const std::string& pattern_name) {
	auto pattern_evaluation = std::unordered_map<uint64_t, PatternEvaluation>();
	
	std::fstream testset(testset_name, std::ios::in);
	std::fstream pattern(pattern_name, std::ios::in);

	std::cout << pattern.tellg() << std::endl;

	std::string pattern_header;
	std::getline(pattern, pattern_header);

	auto header_delta = pattern.tellg();

	char delimiter;
	uint64_t movement;
	double acceleration;

	std::string pattern_line;
	while (std::getline(pattern, pattern_line)) {
		std::istringstream pattern_iss(pattern_line);

		pattern_iss >> movement >> delimiter;

		double min = std::numeric_limits<double>::max();
		double max = std::numeric_limits<double>::min();

		while (!pattern_iss.eof()) {
			pattern_iss >> acceleration >> delimiter;
		
			min = std::min(acceleration, min);
			max = std::max(acceleration, max);
		}

		pattern_evaluation[movement] = PatternEvaluation{min, max};
	}

	for (auto [movement, eval] : pattern_evaluation) {
		double delta = eval.max - eval.min;
		double midle = eval.min + (delta / 2);
		std::cout << movement << " | " << eval.min << " | " << eval.max << " | " << midle << std::endl;
	}

	std::string testset_header;
	std::getline(testset, testset_header);

	auto testset_evaluation = std::unordered_map<uint64_t, PatternEvaluation>();

	uint64_t gender;
	uint64_t index;

	std::string testset_line;
	while (std::getline(testset, testset_line)) {
		std::istringstream testset_iss(testset_line);
		
		testset_iss
			>> movement >> delimiter
			>> gender >> delimiter
			>> index >> delimiter;

		double min = std::numeric_limits<double>::max();
		double max = std::numeric_limits<double>::min();

		while (!testset_iss.eof()) {
			testset_iss >> acceleration >> delimiter;

			min = std::min(acceleration, min);
			max = std::max(acceleration, max);
		}

		double delta = max - min;
		double midle = min + (delta / 2);

		std::cout << movement << " | " << min << " | " << max << " | " << midle << std::endl;

		uint64_t movement_min = 0;
		double close_min = std::numeric_limits<double>::max();

		for (auto [pattern_mov, pattern_eval] : pattern_evaluation) {
			double pattern_delta = pattern_eval.max - pattern_eval.min;
			double pattern_midle = pattern_eval.min + (pattern_delta / 2);
			
			double close =
				(pattern_eval.min - min) +
				(pattern_eval.max - max) + 
				(pattern_midle - midle);

			std::cout << pattern_mov << " | " << pattern_eval.min << " | " << pattern_eval.max << " | " << pattern_midle << std::endl;

			if (close < close_min) {
				close_min = close;
				movement_min = pattern_mov;
			}
		}

		std::cout << movement << " | " << movement_min << std::endl;
	}
}