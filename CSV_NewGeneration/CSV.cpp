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
	set_xyz();
	create_pattern("pattern_x.csv", "trainset_x.csv");
	create_pattern("pattern_y.csv", "trainset_y.csv");
	create_pattern("pattern_z.csv", "trainset_z.csv");
}

int main() {
	classic_stack_xyz();

	return EXIT_SUCCESS;
}