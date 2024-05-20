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
const uint64_t TESTSET_COLUMNS = TESTSET_PROPORTION * TRAINSET_COLUMNS;

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
	ACCLERERATION_Z
};

std::array<std::pair<std::regex, MovementType>, 6> MOVEMENT_REGEX = { {
	{std::regex("dws_(\\d+)"), MovementType::DOWNSTAIR},
	{std::regex("jog_(\\d+)"), MovementType::JOGGING},
	{std::regex("ups_(\\d+)"), MovementType::UPSTAIRS},
	{std::regex("sit_(\\d+)"), MovementType::SIT_DOWN},
	{std::regex("std_(\\d+)"), MovementType::STAND_UP},
	{std::regex("wlk_(\\d+)"), MovementType::WALKING}
} };

std::unordered_map<std::string, RawColumnName> RAW_COLUMN_MAP = {
	{"attitude.roll", ATTITUDE_ROLL},
	{"attitude.pitch", ATTITUDE_PITCH},
	{"attitude.yaw", ATTITUDE_YAW},
	{"gravity.x", GRAVITY_X},
	{"gravity.y", GRAVITY_Y},
	{"gravity.z", GRAVITY_Z},
	{"rotationRate.x", ROTATION_X},
	{"rotationRate.y", ROTATION_Y},
	{"rotationRate.z", ROTATION_Z},
	{"userAcceleration.x", ACCLERERATION_X},
	{"userAcceleration.y", ACCLERERATION_Y},
	{"userAcceleration.z", ACCLERERATION_Z},
};

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

struct EvalutionStats {
	std::array<uint64_t, 6> guess_at = {0};
};

struct RawColumnData {
	RawColumnName name;
	double average;
	double std;
};

struct RunParameter {
	uint64_t trainset_col;
	uint64_t testset_col;
	std::vector<RawColumnName> extract_col;
	ExtremeFunction extreme;
	FilterFunction filter;
};

struct GlobalState {
	std::unordered_map<RawColumnName, std::pair<double, double>> columns_data;
};

void for_file(const fs::path& directory, std::function<void(fs::path)> on_file);

size_t find_directory_type(const std::string& directory_name);
uint64_t find_gender(const uint64_t person_id);
void create_header(std::fstream& output_file, const size_t columns_count);
uint64_t create_set(std::fstream& current_file, const uint64_t line_count, std::fstream& output_file, std::function<bool(double, double, double)> extreme_func);
ProcessError set(std::fstream& trainset, std::fstream& testset, std::function<bool(double, double, double)> extreme_func);
ProcessError phase_zero();

bool is_extreme(const double value, const double average, const double std);
bool is_extreme_z(const double value, const double average, const double std);
bool no_extreme(const double value, const double average, const double std);
ProcessError verification(std::fstream& checkfile, std::function<bool(double, double, double)> extreme_func);
ProcessError phase_one();

ProcessError create_pattern(const std::string& pattern_name, const std::string& trainset_name);
ProcessError phase_two();

std::vector<double> find_acceleration(std::fstream& pattern, const MovementType movement_type);
ProcessError evaluation(const std::string& testset_name, const std::string& pattern_name, const std::string& evaluation_name);
ProcessError phase_three();

ProcessError test();

void create_header_xyz(std::ofstream& output_file, const size_t columns_count);
void set_xyz();
void extract_rawline(RawLine& rawline, std::istringstream& iss);
void extract_setline(SetLine& rawline, std::istringstream& iss);
void extract_setline_core(SetLine& setline, std::istringstream& iss);
void extract_setline_acceleration(SetLine& setline, std::istringstream& iss);
void extract_subjectline(SubjectLine& subjectline, std::istringstream& iss);

void evaluation_xyz();

void evaluation_freq(const std::string& testset_name, const std::string& pattern_name);