#pragma once

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <filesystem>

namespace fs = std::filesystem;

class OutputStream {
public:
	OutputStream(const fs::path filepath, const std::vector<std::string> header);
	void add_line(const std::vector<std::string>& line);
private:
	const fs::path filepath;
};