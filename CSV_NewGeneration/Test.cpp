#include "Global.hpp"
#include <chrono>
#include <thread>
#include <mutex>

static void process_file(const fs::path& current_path, const std::function<void(Line)> on_line) {
	std::ifstream current_file(current_path);

	std::string header;
	std::getline(current_file, header);

	char delimiter;
	Line current_line;

	std::string line;
	while (std::getline(current_file, line)) {
		std::istringstream iss(line);
		iss >>
			current_line.id >> delimiter >>
			current_line.attitude_roll >> delimiter >>
			current_line.attitude_pitch >> delimiter >>
			current_line.attitude_yaw >> delimiter >>
			current_line.gravity_x >> delimiter >>
			current_line.gravity_y >> delimiter >>
			current_line.gravity_z >> delimiter >>
			current_line.rotation_rate_x >> delimiter >>
			current_line.rotation_rate_y >> delimiter >>
			current_line.rotation_rate_z >> delimiter >>
			current_line.user_acceleration_x >> delimiter >>
			current_line.user_acceleration_y >> delimiter >>
			current_line.user_acceleration_z >> delimiter;

		on_line(current_line);
	}
	current_file.close();
}

static void proccess_line(std::istringstream& iss, const std::function<void(Line)> on_line) {
	char delimiter;
	Line current_line;

	iss >>
		current_line.id >> delimiter >>
		current_line.attitude_roll >> delimiter >>
		current_line.attitude_pitch >> delimiter >>
		current_line.attitude_yaw >> delimiter >>
		current_line.gravity_x >> delimiter >>
		current_line.gravity_y >> delimiter >>
		current_line.gravity_z >> delimiter >>
		current_line.rotation_rate_x >> delimiter >>
		current_line.rotation_rate_y >> delimiter >>
		current_line.rotation_rate_z >> delimiter >>
		current_line.user_acceleration_x >> delimiter >>
		current_line.user_acceleration_y >> delimiter >>
		current_line.user_acceleration_z >> delimiter;
	
	on_line(current_line);
}

static void process_file_threaded(const fs::path& current_path, const std::function<void(Line)> on_line) {
	std::ifstream current_file(current_path);

	std::string header;
	std::getline(current_file, header);

	std::string line;
	std::vector<std::istringstream> lines;

	while (std::getline(current_file, line)) {
		lines.push_back(std::istringstream(line));
	}

	std::vector<std::thread> threads;
	std::mutex mtx;

	for (uint64_t index = 0; index < 4; ++index) {
		threads.emplace_back([&on_line, &mtx, &lines]() {
			std::istringstream localLine;
			while (true) {
				std::lock_guard<std::mutex> lock(mtx);
				if (lines.empty()) {
					break;
				}
				localLine = std::move(lines.back());
				lines.pop_back();
				proccess_line(localLine, on_line);
			}
		});
	}
	
	for (auto& thread : threads) {
		thread.join();
	}

	current_file.close();
}

void naive_test() {
	auto start = std::chrono::steady_clock::now();
	get_files(DATA_FOLDERPATH, [&](const fs::path filepath) {
		process_file(filepath, [&](const Line& line) {
		});
	});
	auto end = std::chrono::steady_clock::now();
	std::chrono::duration<double> elapsed_seconds = end - start;
	std::cout << "Time taken " << elapsed_seconds.count() << std::endl;
}

void threaded_function(std::vector <fs::path> filepaths) {	
	for (const fs::path file : filepaths) {
		process_file_threaded(file, [&](const Line& line) {
			
		});
	}
	std::cout << "Finished" << std::endl;
}

void threaded_test(const size_t chunk) {
	std::cout << "Starting: " << chunk << std::endl;

	std::vector<std::thread> threads;
	std::vector<fs::path> all_files;

	auto start = std::chrono::steady_clock::now();
	get_files(DATA_FOLDERPATH, [&](const fs::path filepath) {
		all_files.push_back(filepath);
	});
	size_t delta = all_files.size() / chunk;
	size_t last_index = 0;
	for (size_t index = delta; index <= all_files.size(); index += delta) {
		threads.push_back(std::thread(threaded_function, std::vector(all_files.begin() + last_index, all_files.begin() + index)));
		last_index += delta;
	}

	for (std::thread& thread : threads) {
		thread.join();
	}

	auto end = std::chrono::steady_clock::now();
	std::chrono::duration<double> elapsed_seconds = end - start;
	std::cout << "Time taken " << elapsed_seconds.count() << std::endl;
}

void naive_process_file() {
	auto start = std::chrono::steady_clock::now();
	process_file_threaded(DATA_FOLDERPATH / fs::path("sit_5\\sub_3.csv"), [](const Line& line) {

	});
	auto end = std::chrono::steady_clock::now();
	std::chrono::duration<double> elapsed_seconds = end - start;
	std::cout << "Time taken " << elapsed_seconds.count() << std::endl;
}

void threaded_process_file() {
	auto start = std::chrono::steady_clock::now();
	process_file(DATA_FOLDERPATH / fs::path("sit_5\\sub_3.csv"), [](const Line& line) {

	});
	auto end = std::chrono::steady_clock::now();
	auto elapsed_seconds = end - start;
	std::cout << "Time taken " << elapsed_seconds.count() << std::endl;
} 

int test() {
	naive_test();
	threaded_test(4);
}