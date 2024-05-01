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
	create_pattern("pattern_attitude_roll.csv", "trainset_attitude_roll.csv");
	create_pattern("pattern_attitude_pitch.csv", "trainset_attitude_pitch.csv");
	create_pattern("pattern_attitude_yaw.csv", "trainset_attitude_yaw.csv");
	create_pattern("pattern_gravity_x.csv", "trainset_gravity_x.csv");
	create_pattern("pattern_gravity_y.csv", "trainset_gravity_y.csv");
	create_pattern("pattern_gravity_z.csv", "trainset_gravity_z.csv");
	create_pattern("pattern_rotation_rate_x.csv", "trainset_rotation_rate_x.csv");
	create_pattern("pattern_rotation_rate_y.csv", "trainset_rotation_rate_y.csv");
	create_pattern("pattern_rotation_rate_z.csv", "trainset_rotation_rate_z.csv");
	create_pattern("pattern_user_acceleration_x.csv", "trainset_user_acceleration_x.csv");
	create_pattern("pattern_user_acceleration_y.csv", "trainset_user_acceleration_y.csv");
	create_pattern("pattern_user_acceleration_z.csv", "trainset_user_acceleration_z.csv");
	evaluation_xyz();
}

int main() {
	classic_stack_xyz();

	return EXIT_SUCCESS;
}