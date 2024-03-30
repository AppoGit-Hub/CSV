#include "Global.hpp"

[[nodiscard]] static const MovementType find_directory_type(const fs::path& directory) {
	const auto& directory_name = directory.filename();
	for (const auto& pair : MOVEMENT_REGEX) {
		if (std::regex_match(directory_name.string(), pair.first)) {
			return pair.second;
		}
	}
	throw std::runtime_error("Couldnt find directory type for " + directory.string());
}
[[nodiscard]] static uint64_t find_person_id(const fs::path& filepath) {
	const auto& filename = filepath.filename().string();
	std::smatch matches;
	if (std::regex_search(filename, matches, PERSON_FILE_REGEX)) {
			return std::stoull(matches[1].str());
	}
	throw std::runtime_error("Couldnt find id for file of " + filepath.string());
}

[[nodiscard]] static uint64_t find_gender(const uint64_t person_id, std::ifstream& subjects) noexcept {
	uint64_t code = 0;
	uint64_t weight;
	uint64_t height;
	uint64_t age;
	uint64_t gender = 0;

	char delimiter;

	subjects.clear();
	subjects.seekg(0, std::ios::beg);

	std::string header;
	std::getline(subjects, header);

	std::string line;
	while (std::getline(subjects, line) && code < person_id) {
		std::istringstream iss(line);
		
		iss >>
			code >> delimiter >>
			weight >> delimiter >>
			height >> delimiter >>
			age >> delimiter >>
			gender;
	}

	return gender;
}

static void create_header(std::ofstream& output_file, const size_t columns_count) {
	output_file << "Mouvement" << DELIMITER << "Gender" << DELIMITER << "Index";
	for (size_t index = 0; index < columns_count; index++) {
		output_file << DELIMITER << "Vacc";
	}
	output_file << std::endl;
}

[[nodiscard]] static uint64_t create_set(
	const fs::path& current_path,
	const uint64_t line_count,
	std::ifstream& subjects, 
	std::ofstream& output_file, 
	const uint64_t file_index
) {
	const auto directory_type = find_directory_type(current_path.parent_path());
	const uint64_t person_id = find_person_id(current_path);
	const uint64_t gender = find_gender(person_id, subjects);

	output_file << static_cast<uint64_t>(directory_type) << DELIMITER << gender << DELIMITER << file_index;

	uint64_t total_lines = 0;
	for_line<Line>(current_path, [&](const Line& line) {
		if (total_lines > line_count) {
			output_file << std::endl;
			return total_lines;
		}
		
		double acceleration = sqrt(pow(line.user_acceleration_x, 2) + pow(line.user_acceleration_y, 2) + pow(line.user_acceleration_z, 2));
		output_file << DELIMITER << acceleration;
		total_lines++;
	});
	output_file << std::endl;
	return total_lines;
}

void csv() {
	const auto& data_filepath = fs::path{BASE_FOLDER} / DATA_FOLDER;
	const auto& subject_filepath = fs::path{BASE_FOLDER} / SUBJECT_FILENAME;
	
	std::ifstream subjects_file(subject_filepath);

	std::ofstream trainset_file(TRAINSET_FILENAME, std::ios::app);
	std::ofstream testset_file(TESTSET_FILENAME, std::ios::app);

	create_header(trainset_file, TRAINSET_COLUMNS);
	create_header(testset_file, TESTSET_COLUMNS);

	// untested code
	uint64_t file_index = 0;
	for_file(DATA_FOLDERPATH, [&](const fs::path current_path) {
		try {
			uint64_t train_index = create_set(current_path, TRAINSET_COLUMNS, subjects_file, trainset_file, file_index);
			if (train_index == TRAINSET_COLUMNS) {
				uint64_t test_index = create_set(current_path, TESTSET_COLUMNS, subjects_file, testset_file, file_index);
			}
			std::cout << "Processed file : " << current_path << std::endl;
			file_index++;
		}
		catch (const std::exception& error) {
			std::cout << "Couldnt process file : " << current_path << std::endl;
		}
	});
}