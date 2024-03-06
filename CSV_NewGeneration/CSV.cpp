// frederic.clerin@henallux.be

#include <iostream>
#include <sstream>
#include <fstream>
#include <regex>
#include <array>
#include <string>
#include <filesystem>

namespace fs = std::filesystem;

inline const double TESTSET_PROPORTION = 0.1;

inline const std::string BASE_FOLDER = "archive";
inline const std::string DATA_FOLDER = "data";
inline const std::string SUBJECT_FILENAME = "data_subjects_info.csv";
inline const std::string TRAINSET_FILENAME = "trainset.csv";
inline const std::string TESTSET_FILENAME = "testset.csv";

inline const std::string DELIMITER = ",";
inline const uint64_t TRAINSET_COLUMNS = 600;
inline const uint64_t TESTSET_COLUMNS = TRAINSET_COLUMNS * TESTSET_PROPORTION;

inline const std::regex PERSON_FILE_REGEX("sub_(\\d+).csv");

enum class MovementType : uint64_t {
	Downstair = 1,
	Jogging,
	Upstairs,
	SitDown,
	StandUp,
	Walking,
};

static std::array<std::pair<std::regex, MovementType>, 6> MOVEMENT_REGEX = { {
	{std::regex("dws_(\\d+)"), MovementType::Downstair},
	{std::regex("jog_(\\d+)"), MovementType::Jogging},
	{std::regex("ups_(\\d+)"), MovementType::Upstairs},
	{std::regex("sit_(\\d+)"), MovementType::SitDown},
	{std::regex("std_(\\d+)"), MovementType::StandUp},
	{std::regex("wlk_(\\d+)"), MovementType::Walking}
} };

static const MovementType find_directory_type(const fs::path& directory) noexcept {
	const auto& directory_name = directory.filename();
	for (const auto& pair : MOVEMENT_REGEX) {
		if (std::regex_match(directory_name.string(), pair.first)) {
			return pair.second;
		}
	}
}

static uint64_t find_person_id(const fs::path& filepath) noexcept {
	const auto& filename = filepath.filename().string();
	std::smatch matches;
	if (std::regex_search(filename, matches, PERSON_FILE_REGEX)) {
		if (matches.size() > 1) {
			return std::stoull(matches[1].str());
		}
	}
}

static uint64_t find_gender(const uint64_t person_id, std::ifstream& subjects) {
	uint64_t code = 0;
	uint64_t weight;
	uint64_t height;
	uint64_t age;
	uint64_t gender = 0;

	char delimiter;
	
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

static uint64_t proccess_file(
	uint64_t& file_index,
	uint64_t line_count,
	const fs::path current_path,
	std::ifstream& current_file,
	std::ofstream& output_file, 
	std::ifstream& subjects
) {
	const auto& directory_type = find_directory_type(current_path.parent_path());
	const uint64_t person_id = find_person_id(current_path);
	const uint64_t gender = find_gender(person_id, subjects);
	
	uint64_t id;
	double attitude_roll;
	double attitude_pitch;
	double attitude_yaw;
	double gravity_x;
	double gravity_y;
	double gravity_z;
	double rotationRate_x;
	double rotationRate_y;
	double rotationRate_z;
	double userAcceleration_x;
	double userAcceleration_y;
	double userAcceleration_z;
	
	char delimiter;
	std::string line;

	std::getline(current_file, line);

	output_file << static_cast<uint64_t>(directory_type) << DELIMITER << gender << DELIMITER << file_index;
	
	uint64_t index;
	for (index = 0; index < line_count && std::getline(current_file, line); index++) {
		std::istringstream iss(line);
		iss >>
			id >> delimiter >>
			attitude_roll >> delimiter >>
			attitude_pitch >> delimiter >>
			attitude_yaw >> delimiter >>
			gravity_x >> delimiter >>
			gravity_y >> delimiter >>
			gravity_z >> delimiter >>
			rotationRate_x >> delimiter >>
			rotationRate_y >> delimiter >>
			rotationRate_z >> delimiter >>
			userAcceleration_x >> delimiter >>
			userAcceleration_y >> delimiter >>
			userAcceleration_z >> delimiter;

		double acceleration = sqrt(pow(userAcceleration_x, 2) + pow(userAcceleration_y, 2) + pow(userAcceleration_z, 2));
		output_file << DELIMITER << acceleration;
	}
	output_file << std::endl;

	return index;
}

static void create_header(std::ofstream& output_file, const size_t columns_count) {
	output_file << "Mouvement, Gender, Index";
	for (size_t index = 0; index < columns_count; index++) {
		output_file << ", Vacc";
	}
	output_file << std::endl;
}

static void launch(
	uint64_t& file_index,
	const fs::path& directory,
	std::ofstream& trainset,
	std::ofstream& testset,
	std::ifstream& subjects
) {
	for (const auto& entry : fs::directory_iterator(directory)) {
		const auto& current_path = entry.path();
		if (fs::is_regular_file(current_path)) {
			std::ifstream file(current_path);
			uint64_t train_index = proccess_file(file_index, TRAINSET_COLUMNS, current_path, file, trainset, subjects);
			if (train_index == TRAINSET_COLUMNS) {
				uint64_t test_index = proccess_file(file_index, TESTSET_COLUMNS, current_path, file, testset, subjects);
			}
			file_index++;
		}
		else if (fs::is_directory(current_path)) {
			launch(file_index, current_path, trainset, testset, subjects);
		}
	}

}

int main() {
	const auto& data_filepath = fs::path{BASE_FOLDER} / DATA_FOLDER;
	const auto& subject_filepath = fs::path{BASE_FOLDER} / SUBJECT_FILENAME;
	
	std::ifstream subjects_file(subject_filepath);

	std::ofstream trainset_file(TRAINSET_FILENAME, std::ios::out | std::ios::app);
	std::ofstream testset_file(TESTSET_FILENAME, std::ios::out | std::ios::app);

	create_header(trainset_file, TRAINSET_COLUMNS);
	create_header(testset_file, TESTSET_COLUMNS);

	uint64_t file_index = 1;
	launch(file_index, data_filepath, trainset_file, testset_file, subjects_file);

	return EXIT_SUCCESS;
}