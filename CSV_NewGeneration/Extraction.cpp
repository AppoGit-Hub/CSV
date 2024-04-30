#include "Global.hpp"

void extract_rawline(RawLine& rawline, std::istringstream& iss) {	
	char delimiter;

	iss >>
		rawline.id >> delimiter >>
		rawline.attitude_roll >> delimiter >>
		rawline.attitude_pitch >> delimiter >>
		rawline.attitude_yaw >> delimiter >>
		rawline.gravity_x >> delimiter >>
		rawline.gravity_y >> delimiter >>
		rawline.gravity_z >> delimiter >>
		rawline.rotation_rate_x >> delimiter >>
		rawline.rotation_rate_y >> delimiter >>
		rawline.rotation_rate_z >> delimiter >>
		rawline.user_acceleration_x >> delimiter >>
		rawline.user_acceleration_y >> delimiter >>
		rawline.user_acceleration_z >> delimiter;
};

void extract_setline(SetLine& rawline, std::istringstream& iss) {

}

void extract_setline_core(SetLine& setline, std::istringstream& iss) {
	char delimiter;
	uint64_t movement;
	
	iss >>
		movement >> delimiter >>
		setline.person_id >> delimiter >>
		setline.gender >> delimiter;

	setline.movement_type = static_cast<MovementType>(movement);
}

void extract_setline_acceleration(SetLine& setline, std::istringstream& iss) {

}