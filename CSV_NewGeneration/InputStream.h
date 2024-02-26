#pragma once

#include <iostream>
#include <filesystem>
#include <string>
#include <fstream>

namespace fs = std::filesystem;
typedef std::vector<std::string> HeaderType;

class InputStream {
public:
	InputStream(const fs::path filepath, const char& delimiter, const bool& skip_bom);
	std::pair<size_t, size_t> for_each(void (*lambda)(size_t, size_t));
	size_t extract(void (*lambda)(size_t), const std::vector<std::string>& columns);

private:
	const fs::path filepath;
	const char delimiter;

	std::shared_ptr<HeaderType> header;
	std::streampos header_start;
	std::streampos data_start;

	void inspection(const bool& skip_bom);
};