#include "OutputStream.h"

OutputStream::OutputStream(
	const fs::path filepath, 
	const std::vector<std::string> header
) : filepath(filepath) {
	this->add_line(header);
}

void OutputStream::add_line(
	const std::vector<std::string>& line
) {
	std::ofstream file(this->filepath, std::ios_base::app);
	if (!file.is_open()) {
		std::cerr << "Error: Unable to open file " << this->filepath << std::endl;
	}

	std::string row;
	for (size_t index = 0; index < line.size() - 1; index++) {
		row += line[index] + ", ";
	}

	row += line[line.size() - 1];

	file << row << std::endl;
	file.close();
}