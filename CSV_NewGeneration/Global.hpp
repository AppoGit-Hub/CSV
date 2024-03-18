#pragma once
#include <filesystem>
#include <fstream>
#include <iostream>
#include <forward_list>
#include <thread>
#include <chrono>
#include <ranges>

namespace fs = std::filesystem;

static void get_files(const fs::path& directory, std::forward_list<fs::path>& file_paths) {
	for (const auto& entry : fs::directory_iterator(directory)) {
		const auto& current_path = entry.path();
		if (fs::is_regular_file(current_path)) {
			file_paths.push_front(current_path);
		}
		else if (fs::is_directory(current_path)) {
			get_files(current_path, file_paths);
		}
	}
}