#include "Global.hpp"

void create_header_xyz_pattern(std::fstream& file) {
	file << "Mouvement";
	for (size_t index = 0; index < TRAINSET_COLUMNS; index++) {
		file << DELIMITER << "Vacc";
	}
	file << std::endl;
}