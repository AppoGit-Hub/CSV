#include "Global.hpp"

void classic_stack() {
	std::cout << "Creating Set: " << std::endl;
	phase_zero();
	std::cout << "Creating Verification: " << std::endl;
	//phase_one();
	std::cout << "Creating Pattern: " << std::endl;
	phase_two();
	std::cout << "Creating Evaluation: " << std::endl;
	phase_three();
}

void classic_stack_xyz() {
	std::fstream subjects(SUBJECT_FILEPATH, std::ios::in);

	std::fstream trainset_x("trainset_x.csv", std::ios::out);
	create_header_xyz(trainset_x, TRAINSET_COLUMNS);
	std::fstream testset_x("testset_x.csv", std::ios::out);
	create_header_xyz(testset_x, TESTSET_COLUMNS);
	set_xyz(trainset_x, testset_x, [](const RawLine line) {
		return line.user_acceleration_x;
	});

	std::fstream trainset_y("trainset_y.csv", std::ios::out);
	create_header_xyz(trainset_y, TRAINSET_COLUMNS);
	std::fstream testset_y("testset_y.csv", std::ios::out);
	create_header_xyz(testset_y, TESTSET_COLUMNS);
	set_xyz(trainset_y, testset_y, [](const RawLine line) {
		return line.user_acceleration_y;
	});

	std::fstream trainset_z("trainset_z.csv", std::ios::out);
	create_header_xyz(trainset_z, TRAINSET_COLUMNS);
	std::fstream testset_z("testset_z.csv", std::ios::out);
	create_header_xyz(testset_z, TESTSET_COLUMNS);
	set_xyz(trainset_z, testset_z, [](const RawLine line) {
		return line.user_acceleration_z;
	});
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
		set(trainset, testset, no_extreme);
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

	return EXIT_SUCCESS;
}