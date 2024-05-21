#include "Global.hpp"

void get_data(
	GlobalState& state, 
	const RunParameter& run,
	const std::filesystem::path current_path,
	const uint64_t line_count
) {
	std::fstream file(current_path, std::ios::in);

	std::string header;
	std::getline(file, header);

	uint64_t index = 0;
	uint64_t count = 0;
	RawLine total = {0};

	std::string line;
	while (index < line_count && std::getline(file, line)) {
		RawLine rawline;
		std::istringstream iss(line);
		extract_rawline(rawline, iss);

		total.attitude_roll += rawline.attitude_roll;
		total.attitude_roll += rawline.attitude_pitch;
		total.attitude_yaw += rawline.attitude_yaw;
		total.gravity_x += rawline.gravity_x;
		total.gravity_y += rawline.gravity_y;
		total.gravity_z += rawline.gravity_z;
		total.rotation_rate_x += rawline.rotation_rate_x;
		total.rotation_rate_y += rawline.rotation_rate_y;
		total.rotation_rate_z += rawline.rotation_rate_z;
		total.user_acceleration_x += rawline.user_acceleration_x;
		total.user_acceleration_y += rawline.user_acceleration_y;
		total.user_acceleration_z += rawline.user_acceleration_z;
		count++;
		index++;
	}

	RawLine average = { 0 };
	average.attitude_roll += total.attitude_roll / count;
	average.attitude_pitch += total.attitude_pitch / count;
	average.attitude_yaw += total.attitude_yaw / count;
	average.gravity_x += total.gravity_x / count;
	average.gravity_y += total.gravity_y / count;
	average.gravity_z += total.gravity_z / count;
	average.rotation_rate_x += total.rotation_rate_x / count;
	average.rotation_rate_y += total.rotation_rate_y / count;
	average.rotation_rate_z += total.rotation_rate_z / count;
	average.user_acceleration_x += total.user_acceleration_x / count;
	average.user_acceleration_y += total.user_acceleration_y / count;
	average.user_acceleration_z += total.user_acceleration_z / count;

	file.clear();
	file.seekg(std::ios::beg);

	std::getline(file, header);

	RawLine std = { 0 };
	index = 0;

	while (index < line_count && std::getline(file, line)) {
		RawLine rawline;
		std::istringstream iss(line);
		extract_rawline(rawline, iss);

		std.attitude_roll += pow(rawline.attitude_roll - average.attitude_roll, 2);
		std.attitude_pitch += pow(rawline.attitude_pitch - average.attitude_pitch, 2);
		std.attitude_yaw += pow(rawline.attitude_yaw - average.attitude_yaw, 2);
		std.gravity_x += pow(rawline.gravity_x - average.gravity_x, 2);
		std.gravity_y += pow(rawline.gravity_y - average.gravity_y, 2);
		std.gravity_z += pow(rawline.gravity_z - average.gravity_z, 2);
		std.rotation_rate_x += pow(rawline.rotation_rate_x - average.rotation_rate_x, 2);
		std.rotation_rate_y += pow(rawline.rotation_rate_y - average.rotation_rate_y, 2);
		std.rotation_rate_z += pow(rawline.rotation_rate_z - average.rotation_rate_z, 2);
		std.user_acceleration_x += pow(rawline.user_acceleration_x - average.user_acceleration_x, 2);
		std.user_acceleration_y += pow(rawline.user_acceleration_y - average.user_acceleration_y, 2);
		std.user_acceleration_z += pow(rawline.user_acceleration_z - average.user_acceleration_z, 2);
		index++;
	}

	std.attitude_roll = sqrt(std.attitude_roll / count);
	std.attitude_pitch = sqrt(std.attitude_pitch / count);
	std.attitude_yaw = sqrt(std.attitude_yaw / count);
	std.gravity_x = sqrt(std.gravity_x / count);
	std.gravity_y = sqrt(std.gravity_y / count);
	std.gravity_z = sqrt(std.gravity_z / count);
	std.rotation_rate_x = sqrt(std.rotation_rate_x / count);
	std.rotation_rate_y = sqrt(std.rotation_rate_y / count);
	std.rotation_rate_z = sqrt(std.rotation_rate_z / count);
	std.user_acceleration_x = sqrt(std.user_acceleration_x / count);
	std.user_acceleration_y = sqrt(std.user_acceleration_y / count);
	std.user_acceleration_z = sqrt(std.user_acceleration_z / count);

	state.columns_data = {
		{RawColumnName::ATTITUDE_ROLL, std::pair(average.attitude_roll, std.attitude_roll)},
		{RawColumnName::ATTITUDE_PITCH, std::pair(average.attitude_pitch, std.attitude_pitch)},
		{RawColumnName::ATTITUDE_YAW, std::pair(average.attitude_yaw, std.attitude_yaw)},
		{RawColumnName::GRAVITY_X, std::pair(average.gravity_x, std.gravity_x)},
		{RawColumnName::GRAVITY_Y, std::pair(average.gravity_y, std.gravity_y)},
		{RawColumnName::GRAVITY_Z, std::pair(average.gravity_z, std.gravity_z)},
		{RawColumnName::ROTATION_X, std::pair(average.rotation_rate_x, std.rotation_rate_x)},
		{RawColumnName::ROTATION_Y, std::pair(average.rotation_rate_y, std.rotation_rate_y)},
		{RawColumnName::ROTATION_Z, std::pair(average.rotation_rate_z, std.rotation_rate_z)},
		{RawColumnName::ACCLERERATION_X, std::pair(average.user_acceleration_x, std.user_acceleration_x)},
		{RawColumnName::ACCLERERATION_Y, std::pair(average.user_acceleration_y, std.user_acceleration_y)},
		{RawColumnName::ACCLERERATION_Z, std::pair(average.user_acceleration_z, std.user_acceleration_z)},
	};
}

void csv_dyn(const RunParameter& run, GlobalState& state) {
	std::fstream trainset("trainset.csv", std::ios::out);
	std::fstream testset("testset.csv", std::ios::out);

	create_header(trainset, run.trainset_col);
	create_header(testset, run.testset_col);

	uint64_t file_index = 1;
	for_file(DATA_FOLDERPATH, [&](const fs::path current_path) {
		//std::cout << current_path << std::endl;
		
		get_data(state, run, current_path, run.trainset_col);
		
		const std::string directory_name = current_path.parent_path().filename().string();

		const size_t directory_index = find_directory_type(directory_name);
		if (directory_index == MOVEMENT_REGEX.size())
			return DIRECTORY_TYPE_NOT_FOUND;

		const std::string filepath_name = current_path.string();
		std::smatch matches;
		if (!std::regex_search(filepath_name, matches, PERSON_FILE_REGEX))
			return PERSON_FILE_NOT_FOUND;

		const MovementType directory_type = MOVEMENT_REGEX[directory_index].second;
		const uint64_t person_id = std::stoull(matches[1].str());
		const uint64_t gender = find_gender(person_id);

		std::fstream current_file(current_path);
		if (!current_file.is_open())
			return COUDLNT_OPEN_FILE;

		std::string header;
		std::getline(current_file, header);

		trainset << (directory_index + 1) << DELIMITER << gender << DELIMITER << file_index;
		uint64_t trainset_lines = create_set_dyn(run, state, current_file, trainset, run.trainset_col);

		if (trainset_lines == run.trainset_col) {
			testset << (directory_index + 1) << DELIMITER << gender << DELIMITER << file_index;
			uint64_t testset_lines = create_set_dyn(run, state, current_file, testset, run.testset_col);
		}

		file_index++;
	});
}

void to_columns(
	const std::bitset<static_cast<RawColumnName>(RawColumnName::SIZE)>& bits,
	std::vector<RawColumnName>& columns
) {
	for (size_t flag_index = 0; flag_index < bits.size(); flag_index++) {
		bool bit = bits.test(flag_index);
		if (bit) {
			columns.push_back(static_cast<RawColumnName>(flag_index));
		}
	}
}

void to_bitset(
	const std::vector<RawColumnName>& columns,
	std::bitset<static_cast<RawColumnName>(RawColumnName::SIZE)>& bits
) {
	for (size_t col_index = 0; col_index < columns.size(); col_index++) {
		auto column = columns[col_index];
		bits.set(static_cast<uint64_t>(column));
	}
}

std::array<std::array<uint64_t, 6>, 6> test_combination(const RunParameter& run, GlobalState& state) {
	csv_dyn(run, state);
	phase_two();
	return phase_three();
}

double get_performance(const std::array<std::array<uint64_t, 6>, 6>& results) {
	uint64_t total = 0;
	uint64_t total_right = 0;
	for (size_t eval_index = 0; eval_index < results.size(); eval_index++) {
		total_right += results[eval_index][eval_index];

		for (size_t guess_index = 0; guess_index < results[eval_index].size(); guess_index++) {
			total += results[eval_index][guess_index];
		}
	}
	return double(total_right) / total;
}

void finder() {
	GlobalState state;

	std::fstream combination("combination.csv", std::ios::out);
	combination << "Combination" << DELIMITER << "Result" << std::endl;

	const auto max_names = static_cast<size_t>(RawColumnName::SIZE);
	for (size_t name_index = 0; name_index < pow(2, max_names); ++name_index) {
		const std::bitset<static_cast<uint64_t>(RawColumnName::SIZE)> flags(name_index);
		std::vector<RawColumnName> extract;
		to_columns(flags, extract);

		RunParameter run = RunParameter(TRAINSET_COLUMNS, TRAINSET_COLUMNS, extract, no_extreme);

		std::cout << "Combination: " << flags.to_string() << std::endl;

		auto result = test_combination(run, state);
		view_result(result);

		auto performance = get_performance(result);

		combination << flags.to_string() << DELIMITER << performance * 100 << std::endl;
	}
}