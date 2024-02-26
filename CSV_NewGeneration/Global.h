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

static std::string extract_string(const std::string& source, const char& delimiter, const size_t& occurence) {
	size_t index = source.find(delimiter);
	size_t counter = 0;
	while (index < source.length() && counter < occurence - 1) {
		counter += 1;
		index = source.find(delimiter, index + 1);
	}
	if (index < source.length() && counter < occurence - 1) {
		size_t next_index = source.find(delimiter, index + 1);
		return source.substr(index, next_index);
	}
	return source;
}