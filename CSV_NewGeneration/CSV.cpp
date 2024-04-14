#include "Global.hpp"

int main() {
	std::ifstream subjects(SUBJECT_FILEPATH);
	
	std::ofstream trainset(TRAINSET_FILENAME);
	std::ofstream testset(TESTSET_FILENAME);

	set(subjects, trainset, testset);



	return EXIT_SUCCESS;
}