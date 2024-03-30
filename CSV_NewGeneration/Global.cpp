#include "Global.hpp"

void for_file(const fs::path& directory, std::function<void(fs::path)> on_file) {
	for (const auto& entry : fs::directory_iterator(directory)) {
		const auto& current_path = entry.path();
		if (fs::is_regular_file(current_path)) {
			on_file(current_path);
		}
		else if (fs::is_directory(current_path)) {
			for_file(current_path, on_file);
		}
	}
}
