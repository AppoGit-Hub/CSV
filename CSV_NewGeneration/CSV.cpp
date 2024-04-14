#include "Global.hpp"

int main() {
	std::fstream subjects(SUBJECT_FILEPATH, std::ios::in);
	if (!subjects.is_open()) return EXIT_FAILURE;
	std::fstream trainset(TRAINSET_FILENAME, std::ios::in | std::ios::out);
	if (!trainset.is_open()) return EXIT_FAILURE;
	std::fstream testset(TESTSET_FILENAME, std::ios::in | std::ios::out);
	if (!testset.is_open()) return EXIT_FAILURE;
	std::fstream checkfile(CHECK_FILENAME, std::ios::out);
	if (!checkfile.is_open()) return EXIT_FAILURE;
	std::fstream pattern(PATTERN_FILENAME, std::ios::in | std::ios::out);
	if (!pattern.is_open()) return EXIT_FAILURE;

	set(subjects, trainset, testset);
	verification(checkfile);
	create_pattern(pattern, trainset);
	evaluation(testset, pattern);

	return EXIT_SUCCESS;
}