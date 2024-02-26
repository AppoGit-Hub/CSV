#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <thread>

constexpr int num_threads = 8; // Number of threads for parallel writing

// Function to write data to file
void writeToFile(const std::string& filename, const std::vector<std::string>& data, int thread_id, int num_threads) {
	std::ofstream file(filename, std::ios_base::out | std::ios_base::app);
	if (!file.is_open()) {
		std::cerr << "Error opening file: " << filename << std::endl;
		return;
	}

	// Calculate chunk size for this thread
	int chunk_size = data.size() / num_threads;
	int start = thread_id * chunk_size;
	int end = (thread_id == num_threads - 1) ? data.size() : start + chunk_size;

	// Write data to file
	for (int i = start; i < end; ++i) {
		file << data[i] << "\n"; // Write each string followed by newline
	}

	file.close();
}

int other2() {
	const std::string filename = "output.txt";
	const int data_size = 1000000; // Adjust the size of data as needed
	const int num_threads = 8; // Number of threads for parallel writing

	// Generate some data
	std::vector<std::string> data;
	for (int i = 0; i < data_size; ++i) {
		data.push_back("Data line " + std::to_string(i));
	}

	// Create threads for parallel writing
	std::vector<std::thread> threads;
	for (int i = 0; i < num_threads; ++i) {
		threads.emplace_back(writeToFile, filename, std::ref(data), i, num_threads);
	}

	// Join threads
	for (auto& thread : threads) {
		thread.join();
	}

	std::cout << "Data written to file: " << filename << std::endl;

	return 0;
}
