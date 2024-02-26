#pragma once

#include <iostream>
#include <filesystem>
#include <string>
#include <fstream>

namespace fs = std::filesystem;

class InputStream {
public:
	InputStream(const fs::path filepath, const char& delimiter, const bool& skip_bom);
	size_t get_depth() const noexcept;
	std::string get_at(const std::string& column, const size_t& row_index) const;
	std::vector<std::string> get_row(const size_t& row) const;
	std::string get_column_where(const std::string& column_search, const std::string& column_got, const std::string& value);

private:
	std::vector<std::string> header;
	std::vector<std::vector<std::string>> data;
	
	const fs::path filepath;
	const char delimiter;

	size_t lines_count;

	void from_file(const bool& skip_bom);
};