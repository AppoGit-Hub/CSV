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

namespace fs = std::filesystem;

const std::string DELIMITER = ",";
const std::string BASE_FOLDER = "archive";
const std::string DATA_FOLDER = "data";
const std::string CHECK_FILENAME = "verification.csv";
const std::string SUBJECT_FILENAME = "data_subjects_info.csv";
const std::string TRAINSET_FILENAME = "trainset.csv";
const std::string TESTSET_FILENAME = "testset.csv";

const auto DATA_FOLDERPATH = fs::path{ BASE_FOLDER } / DATA_FOLDER;

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
};

static std::array<std::pair<std::regex, MovementType>, 6> MOVEMENT_REGEX = { {
	{std::regex("dws_(\\d+)"), MovementType::DOWNSTAIR},
	{std::regex("jog_(\\d+)"), MovementType::JOGGING},
	{std::regex("ups_(\\d+)"), MovementType::UPSTAIRS},
	{std::regex("sit_(\\d+)"), MovementType::SIT_DOWN},
	{std::regex("std_(\\d+)"), MovementType::STAND_UP},
	{std::regex("wlk_(\\d+)"), MovementType::WALKING}
} };

struct Line {
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

static void get_files(const fs::path& directory, std::function<void(fs::path)> on_file) {
	for (const auto& entry : fs::directory_iterator(directory)) {
		const auto& current_path = entry.path();
		if (fs::is_regular_file(current_path)) {
			on_file(current_path);
		}
		else if (fs::is_directory(current_path)) {
			get_files(current_path, on_file);
		}
	}
}