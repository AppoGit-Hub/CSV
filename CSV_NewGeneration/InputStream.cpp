#include "InputStream.h"

InputStream::InputStream(
	const fs::path filepath, 
	const char& delimiter = ',', 
	const bool& skip_bom = false
) : filepath(filepath), delimiter(delimiter), lines_count(0) {
	this->from_file(skip_bom);
}

size_t InputStream::get_depth(
) const noexcept {
	return this->lines_count;
}

std::string InputStream::get_at(
	const std::string& column, 
	const size_t& row_index
) const {
	const auto& column_iter = std::find(this->header.begin(), this->header.end(), column);
	if (column_iter != this->header.end()) {
		const auto& column_index = std::distance(this->header.begin(), column_iter);
		const auto& row = this->get_row(row_index);
		if (column_index < row.size()) {
			return row[column_index];
		}
	}
}

std::vector<std::string> InputStream::get_row(
	const size_t& row
) const {
	if (row < this->data.size()) {
		return this->data[row];
	}
	else {
		std::cout << "error" << std::endl;
	}
}

std::string InputStream::get_column_where(
	const std::string& column_search, 
	const std::string& column_got, 
	const std::string& value
) {
	const auto& column_search_iter = std::find(this->header.begin(), this->header.end(), column_search);
	const auto& column_got_iter = std::find(this->header.begin(), this->header.end(), column_got);
	if (column_search_iter != this->header.end() && column_got_iter != this->header.end()) {
		const auto& column_search_index = std::distance(this->header.begin(), column_search_iter);
		const auto& column_got_index = std::distance(this->header.begin(), column_got_iter);

		std::vector<std::string> row;
		for (size_t index = 0; index < this->get_depth(); index++) {
			const auto& current_row = this->get_row(index);
			if (current_row[column_search_index] == value) {
				row = current_row;
			}
		}

		if (!row.empty()) {
			return row[column_got_index];
		}
	}
}


void InputStream::from_file(
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

	std::string line;
	if (getline(file, line)) {
		std::stringstream ss(line);
		std::string cell;

		while (getline(ss, cell, ',')) {
			this->header.push_back(cell);
		}
	}

	while (getline(file, line)) {
		std::stringstream ss(line);
		std::string cell;
		std::vector<std::string> row;

		while (std::getline(ss, cell, this->delimiter)) {
			row.push_back(cell);
		}

		this->data.push_back(row);
		this->lines_count += 1;

		if (row.size() != header.size()) {
			std::cerr << "Expected: " << header.size() << ", Got: " << row.size() << std::endl;
		}
	}
}

