#include "Global.h"
#include "InputStream.h"

InputStream::InputStream(
	const fs::path filepath, 
	const char& delimiter = ',', 
	const bool& skip_bom = false
) : filepath(filepath), delimiter(delimiter) {
	this->header = std::make_shared<HeaderType>();
	this->inspection(skip_bom);
}

std::pair<size_t, size_t> InputStream::for_each(void (*lambda)(size_t, size_t)) {
	std::ifstream file(this->filepath);
	
	file.seekg(this->data_start);

	size_t line_index = 0;
	size_t max_rows = 0;
	std::string line;
	while (getline(file, line)) {
		std::stringstream ss(line);
		std::string cell;

		size_t row_index = 0;
		while (std::getline(ss, cell, this->delimiter)) {
			lambda(line_index, row_index);
			max_rows = std::max(max_rows, row_index);
			row_index += 1;
		}
		line_index += 1;
	}

	file.close();
	return std::make_pair(line_index, max_rows);
}


size_t InputStream::extract(void (*lambda)(size_t), const std::vector<std::string>& columns) {
	std::ifstream file(this->filepath);

	file.seekg(this->data_start);

	size_t line_index = 0;
	std::string line;
	while (getline(file, line)) {
		std::stringstream ss(line);
		std::string cell;
		
		/*
		for (int i = 0; i < 9; ++i) {
			std::getline(file, line);
		}

		while (std::getline(ss, cell, this->delimiter)) {
		}
		*/


		line_index += 1;
	}

	file.close();
	return line_index;
}

void InputStream::inspection(
	const bool& skip_bom = false
) {
	std::ifstream file(this->filepath);

	if (!file.is_open()) {
		std::cerr << "Failed to open the file." << std::endl;
	}

	if (skip_bom) {
		char bom[3];
		file.read(bom, 3);
	}

	this->header_start = file.tellg();

	std::string line;
	if (getline(file, line)) {
		std::stringstream ss(line);
		std::string cell;

		while (getline(ss, cell, this->delimiter)) {
			this->header->push_back(cell);
		}
	}

	this->data_start = file.tellg();

	file.close();
}