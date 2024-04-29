#pragma once
#include <filesystem>
#include <fstream>
#include <iostream>
#include <forward_list>
#include <thread>
#include <chrono>
#include <ranges>
#include <functional>
#include <sstream>
#include <regex>
#include <array>
#include <string>
#include <optional>
#include <stdexcept>
#include <math.h>
#include <unordered_map>
#include <optional>
#include <cassert>

namespace fs = std::filesystem;

const std::string DELIMITER = ",";
const std::string BASE_FOLDER = "archive";
const std::string DATA_FOLDER = "data";
const std::string CHECK_FILENAME = "extreme.csv";
const std::string PATTERN_FILENAME = "pattern.csv";
const std::string EVALUATION_FILENAME = "evaluation.csv";
const std::string SUBJECT_FILENAME = "data_subjects_info.csv";
const std::string TRAINSET_FILENAME = "trainset.csv";
const std::string TESTSET_FILENAME = "testset.csv";
const std::string CLEANED_TRAINSET_FILENAME = "cleanedtrainset.csv";

const auto DATA_FOLDERPATH = fs::path{ BASE_FOLDER } / DATA_FOLDER;
const auto SUBJECT_FILEPATH = fs::path{ BASE_FOLDER } / SUBJECT_FILENAME;

inline const double TESTSET_PROPORTION = 0.1;

inline const double AVERAGE_X = 0.00096087;
inline const double AVERAGE_Y = 0.05525659;
inline const double AVERAGE_Z = 0.0352192;

inline const double STANDARD_DEVIATION_X = 0.38875666;
inline const double STANDARD_DEVIATION_Y = 0.61937128;
inline const double STANDARD_DEVIATION_Z = 0.4300345;

inline const uint64_t TRAINSET_COLUMNS = 600;
const uint64_t TESTSET_COLUMNS = TRAINSET_COLUMNS * TESTSET_PROPORTION;

const std::regex PERSON_FILE_REGEX("sub_(\\d+).csv");

enum class MovementType : uint64_t {
	DOWNSTAIR = 1,
	JOGGING,
	UPSTAIRS,
	SIT_DOWN,
	STAND_UP,
	WALKING,
	SIZE,
};

enum ProcessError : uint64_t {
	NO_ERROR,
	DIRECTORY_TYPE_NOT_FOUND,
	PERSON_FILE_NOT_FOUND,
	COUDLNT_OPEN_FILE
};

static std::array<std::pair<std::regex, MovementType>, 6> MOVEMENT_REGEX = { {
	{std::regex("dws_(\\d+)"), MovementType::DOWNSTAIR},
	{std::regex("jog_(\\d+)"), MovementType::JOGGING},
	{std::regex("ups_(\\d+)"), MovementType::UPSTAIRS},
	{std::regex("sit_(\\d+)"), MovementType::SIT_DOWN},
	{std::regex("std_(\\d+)"), MovementType::STAND_UP},
	{std::regex("wlk_(\\d+)"), MovementType::WALKING}
} };

struct RawLine {
	uint64_t id;
	double attitude_roll;
	double attitude_pitch;
	double attitude_yaw;
	double gravity_x;
	double gravity_y;
	double gravity_z;
	double rotation_rate_x;
	double rotation_rate_y;
	double rotation_rate_z;
	double user_acceleration_x;
	double user_acceleration_y;
	double user_acceleration_z;

	static const RawLine extract(std::istringstream& iss) {
		char delimiter;
		RawLine line;

		iss >>
			line.id >> delimiter >>
			line.attitude_roll >> delimiter >>
			line.attitude_pitch >> delimiter >>
			line.attitude_yaw >> delimiter >>
			line.gravity_x >> delimiter >>
			line.gravity_y >> delimiter >>
			line.gravity_z >> delimiter >>
			line.rotation_rate_x >> delimiter >>
			line.rotation_rate_y >> delimiter >>
			line.rotation_rate_z >> delimiter >>
			line.user_acceleration_x >> delimiter >>
			line.user_acceleration_y >> delimiter >>
			line.user_acceleration_z >> delimiter;

		return line;
	};
};

struct SetLine {
	MovementType movement_type;
	uint64_t person_id;
	uint64_t gender;
	std::vector<double> accelerations;
};

struct SetResult {
	uint64_t error;
};

struct EvalutionStats {
	uint64_t right;
	uint64_t wrong;
};

void for_file(const fs::path& directory, std::function<void(fs::path)> on_file);

size_t find_directory_type(const std::string& directory_name);
uint64_t find_gender(std::fstream& subjects, const uint64_t person_id) ;
void create_header(std::fstream& output_file, const size_t columns_count);
uint64_t create_set(std::fstream& current_file, const uint64_t line_count, std::fstream& output_file, std::function<bool(double, double, double)> extreme_func);
ProcessError set(std::fstream& subjects, std::fstream& trainset, std::fstream& testset, std::function<bool(double, double, double)> extreme_func);
ProcessError phase_zero();

bool is_extreme(const double value, const double average, const double std);
bool is_extreme_z(const double value, const double average, const double std);
bool no_extreme(const double value, const double average, const double std);
ProcessError verification(std::fstream& checkfile, std::function<bool(double, double, double)> extreme_func);
ProcessError phase_one();

ProcessError create_pattern(std::fstream& pattern, std::fstream& trainset);
ProcessError phase_two();

std::vector<double> find_acceleration(std::fstream& pattern, const MovementType movement_type);
ProcessError evaluation(std::fstream& testset, std::fstream& pattern);
ProcessError phase_three();

ProcessError test();