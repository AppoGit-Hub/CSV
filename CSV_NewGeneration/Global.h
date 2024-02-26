#pragma once

#include <string>
#include <regex>
#include <array>

inline const uint64_t THREAD_COUNT = 8;

inline const std::string BASE_FOLDER = "archive";
inline const std::string DATA_FOLDER = "data";
inline const std::string SUBJECT_FILENAME = "data_subjects_info.csv";

inline const std::regex PERSON_FILE_REGEX("sub_(\\d+).csv");

enum class MovementType {
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