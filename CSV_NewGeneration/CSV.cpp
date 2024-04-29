#include "Global.hpp"

void classic_stack() {
	phase_zero();
	phase_one();
	phase_two();
	phase_three();
}

void combinaition_stack() {
	std::fstream subjects(SUBJECT_FILEPATH, std::ios::in);

	{
		std::string trainset_name = "test_trainset_no.csv";
		std::string testset_name = "test_testset_no.csv";
		std::string pattern_name = "test_pattern_no.csv";
		
		std::fstream trainset("test_trainset_no.csv", std::ios::in | std::ios::out);
		std::fstream testset("test_testset_no.csv", std::ios::in | std::ios::out);
		std::fstream pattern("test_pattern_no.csv", std::ios::in | std::ios::out);

		

		std::cout <<
			"trainset: " << trainset.is_open() << " | " <<
			"testset: " << testset.is_open() << " | " <<
			"pattern: " << pattern.is_open() << " | " << std::endl;

		std::cout << "Before: " << std::endl;
		std::cout
			<< subjects.tellg() << " | "
			<< trainset.tellg() << " | "
			<< testset.tellg() << std::endl;

		std::cout << "Set: " << std::endl;
		set(subjects, trainset, testset, no_extreme);
		std::cout
			<< subjects.tellg() << " | "
			<< trainset.tellg() << " | "
			<< testset.tellg() << std::endl;

		std::cout << "Pattern: " << std::endl;
		create_pattern(pattern, trainset);
		std::cout
			<< subjects.tellg() << " | "
			<< trainset.tellg() << " | "
			<< testset.tellg() << std::endl;

		std::cout << "Evaluation: " << std::endl;
		evaluation(testset, pattern);
		std::cout
			<< subjects.tellg() << " | "
			<< trainset.tellg() << " | "
			<< testset.tellg() << std::endl;

		trainset.close();
		testset.close();
		pattern.close();
	}
}

int main() {
	phase_zero();
	phase_two();


	return EXIT_SUCCESS;
}