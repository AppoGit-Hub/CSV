#include "Global.hpp"

int main() {
	std::fstream subjects;
	std::fstream trainset;
	std::fstream testset;
	std::fstream checkfile;
	std::fstream pattern;
	
	set(subjects, trainset, testset);
	verification(checkfile);
	create_pattern(pattern, trainset);
	evaluation(testset, pattern);

	return EXIT_SUCCESS;
}