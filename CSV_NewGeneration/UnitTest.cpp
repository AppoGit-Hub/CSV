#include "Global.hpp"

ProcessError test_create_set() {
	std::fstream test_in("test_set_in.csv", std::ios::in);
	if (!test_in.is_open())
		return COUDLNT_OPEN_FILE;

	std::fstream test_out("test_set_out.csv", std::ios::out);
	if (!test_out.is_open())
		return COUDLNT_OPEN_FILE;

	uint64_t columns_count = 15;

	test_out << "Mouvement" << DELIMITER << "Gender" << DELIMITER << "Index";
	for (size_t index = 0; index < columns_count; index++) {
		test_out << DELIMITER << "Vacc";
	}
	test_out << std::endl;
	
	std::string header;
	std::getline(test_in, header);

	test_out << 1 << DELIMITER << 1 << DELIMITER << 1;
	create_set(test_in, columns_count, test_out, no_extreme);

	return NO_ERROR;
}

uint64_t test_set_out() {
	std::fstream test_out("test_set_out.csv", std::ios::in);
	if (!test_out.is_open())
		return COUDLNT_OPEN_FILE;

	std::string header;
	std::getline(test_out, header);

	std::string line;
	std::getline(test_out, line);
	std::istringstream iss(line);


	char delimiter;

	uint64_t movement;
	uint64_t person_id;
	uint64_t gender;
	double acceleration;

	iss >> movement >> delimiter 
		>> person_id >> delimiter 
		>> gender >> delimiter;

	iss >> acceleration;

	std::cout << acceleration << std::endl;
}

ProcessError test_verification_extreme() {
	std::fstream checkfile("test_check.csv", std::ios::out);
	if (!checkfile.is_open())
		return COUDLNT_OPEN_FILE;

	verification(checkfile, is_extreme);

	std::fstream checkfile_z("test_check_z.csv", std::ios::out);
	if (!checkfile.is_open())
		return COUDLNT_OPEN_FILE;

	verification(checkfile_z, is_extreme_z);
}

ProcessError test_fstream() {
	std::fstream testa("testa.csv", std::ios::out);
	std::cout << "Test A: " << std::endl;
	std::cout << testa.is_open() << std::endl;
	testa << "testa" << std::endl;
	std::cout << testa.is_open() << std::endl;

	std::fstream testb("testb.csv", std::ios::in);
	std::cout << "Test B: " << std::endl;
	std::cout << testb.is_open() << std::endl;
	testb.open("testb.csv", std::ios::in);
	testb << "testb" << std::endl;
	std::cout << testb.is_open() << std::endl;

	std::fstream testc("testc.csv", std::ios::in | std::ios::out);
	std::cout << "Test C: " << std::endl;
	std::cout << testc.is_open() << std::endl;
	testc.open("testc.csv", std::ios::in | std::ios::out);
	testc << "testc" << std::endl;
	std::cout << testc.is_open() << std::endl;

	return NO_ERROR;
}

ProcessError test() {
	test_create_set();
	test_set_out();
	test_verification_extreme();

	return NO_ERROR;
}