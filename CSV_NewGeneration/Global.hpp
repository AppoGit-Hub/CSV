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
#include <cmath>
#include <utility>
#include <algorithm>
#include <bitset>

namespace fs = std::filesystem;

#define LOGFILE 0
#define EVALUTIONFILE 1

const std::string DELIMITER = ",";
const std::string BASE_FOLDER = "archive";
const std::string DATA_FOLDER = "data";
const std::string CHECK_FILENAME = "extreme.csv";
const std::string PATTERN_FILENAME = "pattern.csv";
const std::string EVALUATION_FILENAME = "evaluation.csv";
const std::string SUBJECT_FILENAME = "data_subjects_info.csv";
const std::string TRAINSET_FILENAME = "trainset.csv";
const std::string TESTSET_FILENAME = "testset.csv";

const auto DATA_FOLDERPATH = std::filesystem::path{ BASE_FOLDER } / DATA_FOLDER;
const auto SUBJECT_FILEPATH = fs::path{ BASE_FOLDER } / SUBJECT_FILENAME;

inline const double TESTSET_PROPORTION = 0.1;

inline const double AVERAGE_X = 0.00096087;
inline const double AVERAGE_Y = 0.05525659;
inline const double AVERAGE_Z = 0.0352192;

inline const double STANDARD_DEVIATION_X = 0.38875666;
inline const double STANDARD_DEVIATION_Y = 0.61937128;
inline const double STANDARD_DEVIATION_Z = 0.4300345;

inline const uint64_t TRAINSET_COLUMNS = 600;
const uint64_t TESTSET_COLUMNS = TRAINSET_COLUMNS;

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

enum RawColumnName : uint64_t {
	ATTITUDE_ROLL,
	ATTITUDE_PITCH,
	ATTITUDE_YAW,
	GRAVITY_X,
	GRAVITY_Y,
	GRAVITY_Z,
	ROTATION_X,
	ROTATION_Y,
	ROTATION_Z,
	ACCLERERATION_X,
	ACCLERERATION_Y,
	ACCLERERATION_Z,
	SIZE
};


static std::array<std::pair<std::regex, MovementType>, 6> MOVEMENT_REGEX = { {
	{std::regex("dws_(\\d+)"), MovementType::DOWNSTAIR},
	{std::regex("jog_(\\d+)"), MovementType::JOGGING},
	{std::regex("ups_(\\d+)"), MovementType::UPSTAIRS},
	{std::regex("sit_(\\d+)"), MovementType::SIT_DOWN},
	{std::regex("std_(\\d+)"), MovementType::STAND_UP},
	{std::regex("wlk_(\\d+)"), MovementType::WALKING}
} };

/// <summary>
/// Detects extemes values
/// </summary>
using ExtremeFunction = std::function<bool(
	double value, 
	double average, 
	double std
)>;

/// <summary>
/// Filter the line of raw data
/// </summary>
using FilterFunction = std::function<void(
	std::vector<std::pair<double, RawColumnName>>,
	ExtremeFunction, 
	std::fstream
)>;

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
};

struct SetLine {
	MovementType movement_type;
	uint64_t person_id;
	uint64_t gender;
	std::vector<double> accelerations;
};

struct SubjectLine {
	uint64_t code;
	uint64_t weight;
	uint64_t height;
	uint64_t age;
	uint64_t gender;
};

struct RunParameter {
	uint64_t trainset_col;
	uint64_t testset_col;
	std::vector<RawColumnName> extract_col;
	ExtremeFunction extreme;
};

// Global.cpp
void for_file(
	const fs::path& directory, 
	std::function<void(fs::path)> on_file
);

// Set.cpp
size_t find_directory_type(
	const std::string& directory_name
);
uint64_t find_gender(
	const uint64_t person_id
);
void create_header(
	std::fstream& output_file, 
	const size_t columns_count
);
void filter_justone(
	const double acceleration_x,
	const double acceleration_y,
	const double acceleration_z,
	ExtremeFunction extreme_func,
	std::fstream& output_file
);
void filter_toaverage(
	double acceleration_x,
	double acceleration_y,
	double acceleration_z,
	ExtremeFunction extreme_func,
	std::fstream& output_file
);
uint64_t create_set(
	std::fstream& current_file, 
	const uint64_t line_count, 
	std::fstream& output_file, 
	ExtremeFunction extreme_func
);
void set(
	std::fstream& trainset, 
	std::fstream& testset, 
	ExtremeFunction extreme_func
);
void phase_zero();

// Verification.cpp
bool is_extreme(
	const double value, 
	const double average, 
	const double std
);
bool is_extreme_z(
	const double value, 
	const double average, 
	const double std
);
bool no_extreme(
	const double value, 
	const double average, 
	const double std
);
void verification(
	std::fstream& checkfile, 
	ExtremeFunction extreme_func
);
void phase_one();

// Pattern.cpp
void create_pattern(
	const std::string& pattern_name,
	const std::string& trainset_name
);
void phase_two();

// Evaluation.cpp
void view_result(
	const std::vector<std::vector<double>>& result,
	const std::string evalution_filename
);
std::vector<double> find_acceleration(
	std::fstream& pattern,
	const MovementType movement_type
);
std::array<std::array<uint64_t, 6>, 6> evaluation(
	const std::string& testset_name,
	const std::string& pattern_name
);
std::array<std::array<uint64_t, 6>, 6> phase_three();

// Extraction.cpp
void extract_rawline(
	RawLine& rawline, 
	std::istringstream& iss
);
void extract_setline(
	SetLine& rawline, 
	std::istringstream& iss
);
void extract_setline_core(
	SetLine& setline, 
	std::istringstream& iss
);
void extract_setline_acceleration(
	SetLine& setline, 
	std::istringstream& iss
);
void extract_subjectline(
	SubjectLine& subjectline, 
	std::istringstream& iss
);

// CSV.cpp
void classic_stack();
void to_columns(
	const std::bitset<static_cast<RawColumnName>(RawColumnName::SIZE)>& bits,
	std::vector<RawColumnName>& columns
);
void to_bitset(
	const std::vector<RawColumnName>& columns,
	std::bitset<static_cast<RawColumnName>(RawColumnName::SIZE)>& bits
);
double get_performance(
	const std::vector<std::vector<double>>& results
);
uint64_t create_set_matrix(
	const std::vector<std::vector<double>>& file_matrix,
	const uint64_t start_row,
	const uint64_t rows,
	const uint64_t columns,
	const RunParameter& run,
	const std::unordered_map<RawColumnName, std::pair<double, double>>& columns_data,
#if LOGFILE
	std::fstream& output,
#endif
	const size_t line_count,
	std::vector<double>& setrow,
	const double max_value
);
std::vector<std::vector<double>> do_run(
	const RunParameter& run, 
	const std::string& trainset_filename, 
	const std::string& testset_filename, 
	const std::string& pattern_filename, 
	const std::string& evaluation_filename
);