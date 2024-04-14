#include "Global.hpp"

int main() {
	std::fstream subjects(SUBJECT_FILEPATH, std::ios::in);
	std::fstream trainset(TRAINSET_FILENAME, std::ios::in | std::ios::out);
	std::fstream testset(TESTSET_FILENAME, std::ios::in | std::ios::out);
	set(subjects, trainset, testset);

	std::fstream checkfile(CHECK_FILENAME, std::ios::out);
	verification(checkfile);

	return EXIT_SUCCESS;
}