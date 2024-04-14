#include "Global.hpp"

int main() {
	/*
	std::fstream subjects(SUBJECT_FILEPATH);
	std::fstream trainset(TRAINSET_FILENAME);
	std::fstream testset(TESTSET_FILENAME);

	set(subjects, trainset, testset);
	*/

	std::fstream checkfile(CHECK_FILENAME);

	verification(checkfile);

	return EXIT_SUCCESS;
}