#include "global.hpp"

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
) {
	uint64_t lines_explored = 0;
	for (size_t row_index = start_row; row_index < rows && lines_explored < line_count; row_index++, lines_explored++) {
		bool is_extreme = false;
		double total = 0;

		for (size_t column_index = 0; column_index < columns; column_index++) {
			RawColumnName column_name = static_cast<RawColumnName>(column_index);

			auto egaler = [&](RawColumnName name) { return name == column_name; };

			auto found_col = std::find_if(run.extract_col.begin(), run.extract_col.end(), egaler);
			if (found_col != run.extract_col.end()) {
				auto found_name = columns_data.find(column_name);
				if (found_name == columns_data.end()) {
					std::cerr << "Couldnt find column data" << std::endl;
				}
				auto column_data = columns_data.at(column_name);

				double value = file_matrix[row_index][column_index + 1];
				is_extreme |= run.extreme(value, column_data.first, column_data.second);

				total += pow(value, 2);
			}
		}

		if (!is_extreme) {
			double acceleration = sqrt(total);
			if (acceleration != max_value) {
				setrow[lines_explored + 1] = acceleration;
#if LOGFILE
				output << DELIMITER << acceleration;
#endif
			}
			else {
#if LOGFILE
				output << DELIMITER;
#endif
			}
		}
	}
#if LOGFILE
	output << std::endl;
#endif
	return lines_explored;
}

std::vector<std::vector<double>> do_run(
	const RunParameter& run,
#if LOGFILE
	const std::string& trainset_filename,
	const std::string& testset_filename,
	const std::string& pattern_filename,
#endif
	const std::string& evaluation_filename
) {
	constexpr double DOUBLE_INF = std::numeric_limits<double>::infinity();
	std::unordered_map<uint64_t, uint64_t> sensitives_files;

	std::vector<std::vector<double>> trainset_matrix(360, std::vector<double>(run.trainset_col + 1, 0));
#if LOGFILE
	std::fstream trainset(trainset_filename, std::ios::out);
	if (!trainset.is_open())
		std::cerr << "Couldnt open file: " << "trainset.csv" << std::endl;
#endif
	std::vector<std::vector<double>> testset_matrix(360, std::vector<double>(run.testset_col + 1, 0));
#if LOGFILE
	std::fstream testset(testset_filename, std::ios::out);
	if (!testset.is_open())
		std::cerr << "Couldnt open file: " << "testset.csv" << std::endl;
#endif
	std::vector<std::vector<double>> pattern_matrix(6, std::vector<double>(run.trainset_col + 1, 0));
#if LOGFILE
	std::fstream pattern(pattern_filename, std::ios::out);
	if (!testset.is_open())
		std::cerr << "Couldnt open file: " << "pattern.csv" << std::endl;
#endif
	std::vector<std::vector<double>> evaluation_matrix(6, std::vector<double>(6, 0));
#if  EVALUTIONFILE
	std::fstream evaluation(evaluation_filename, std::ios::out);
	if (!evaluation.is_open())
		std::cerr << "Couldnt open file: " << "pattern.csv" << std::endl;
#endif
#if LOGFILE
	create_header(trainset, run.trainset_col);
	create_header(testset, run.testset_col);
#endif
#if LOGFILE
	pattern << "Mouvement";
	for (size_t index = 0; index < run.trainset_col; index++) {
		pattern << DELIMITER << "Vacc";
	}
	pattern << std::endl;
#endif
#if EVALUTIONFILE
	evaluation
		<< "Mouvement" << DELIMITER
		<< "1" << DELIMITER
		<< "2" << DELIMITER
		<< "3" << DELIMITER
		<< "4" << DELIMITER
		<< "5" << DELIMITER
		<< "6" << DELIMITER
		<< std::endl;
#endif

	const uint64_t columns = 13;
	const uint64_t total_rows = run.trainset_col + run.testset_col;

	uint64_t file_index = 1;
	for_file(DATA_FOLDERPATH, [&](const fs::path& current_path) {
		std::vector<std::vector<double>> file_matrix(total_rows, std::vector<double>(columns, DOUBLE_INF));

		std::fstream file(current_path, std::ios::in);

		std::string header;
		std::getline(file, header);

		size_t file_lines = 0;
		size_t row_index = 0;
		std::string line;
		while (row_index < total_rows && std::getline(file, line)) {
			std::string value;
			std::stringstream ss(line);
			size_t column_index = 0;

			while (column_index < columns && std::getline(ss, value, ',')) {
				file_matrix[row_index][column_index] = std::stod(value);
				column_index++;
			}
			row_index++;
			file_lines++;
		}

		if (file_lines < total_rows) {
			sensitives_files.insert(std::pair(file_index - 1, file_lines));
			//std::cout << "File index " << file_index << " - " << current_path << " has " << file_lines << " lines" << std::endl;
		}

		// create averages and std
		RawLine total = { 0 };
		for (size_t row_index = 0; row_index < total_rows; row_index++) {
			auto row = file_matrix[row_index];

			total.attitude_roll += row[RawColumnName::ATTITUDE_ROLL + 1];
			total.attitude_pitch += row[RawColumnName::ATTITUDE_PITCH + 1];
			total.attitude_yaw += row[RawColumnName::ATTITUDE_YAW + 1];
			total.gravity_x += row[RawColumnName::GRAVITY_X + 1];
			total.gravity_y += row[RawColumnName::GRAVITY_Y + 1];
			total.gravity_z += row[RawColumnName::GRAVITY_Z + 1];
			total.rotation_rate_x += row[RawColumnName::ROTATION_X + 1];
			total.rotation_rate_y += row[RawColumnName::ROTATION_Y + 1];
			total.rotation_rate_z += row[RawColumnName::ROTATION_Z + 1];
			total.user_acceleration_x += row[RawColumnName::ACCLERERATION_X + 1];
			total.user_acceleration_y += row[RawColumnName::ACCLERERATION_Y + 1];
			total.user_acceleration_z += row[RawColumnName::ACCLERERATION_Z + 1];
		}

		RawLine average = { 0 };
		average.attitude_roll += total.attitude_roll / total_rows;
		average.attitude_pitch += total.attitude_pitch / total_rows;
		average.attitude_yaw += total.attitude_yaw / total_rows;
		average.gravity_x += total.gravity_x / total_rows;
		average.gravity_y += total.gravity_y / total_rows;
		average.gravity_z += total.gravity_z / total_rows;
		average.rotation_rate_x += total.rotation_rate_x / total_rows;
		average.rotation_rate_y += total.rotation_rate_y / total_rows;
		average.rotation_rate_z += total.rotation_rate_z / total_rows;
		average.user_acceleration_x += total.user_acceleration_x / total_rows;
		average.user_acceleration_y += total.user_acceleration_y / total_rows;
		average.user_acceleration_z += total.user_acceleration_z / total_rows;

		RawLine std = { 0 };
		for (size_t row_index = 0; row_index < total_rows; row_index++) {
			auto row = file_matrix[row_index];

			std.attitude_roll += pow(row[RawColumnName::ATTITUDE_ROLL + 1] - average.attitude_roll, 2);
			std.attitude_pitch += pow(row[RawColumnName::ATTITUDE_PITCH + 1] - average.attitude_pitch, 2);
			std.attitude_yaw += pow(row[RawColumnName::ATTITUDE_YAW + 1] - average.attitude_yaw, 2);
			std.gravity_x += pow(row[RawColumnName::GRAVITY_X + 1] - average.gravity_x, 2);
			std.gravity_y += pow(row[RawColumnName::GRAVITY_Y + 1] - average.gravity_y, 2);
			std.gravity_z += pow(row[RawColumnName::GRAVITY_Z + 1] - average.gravity_z, 2);
			std.rotation_rate_x += pow(row[RawColumnName::ROTATION_X + 1] - average.rotation_rate_x, 2);
			std.rotation_rate_y += pow(row[RawColumnName::ROTATION_Y + 1] - average.rotation_rate_y, 2);
			std.rotation_rate_z += pow(row[RawColumnName::ROTATION_Z + 1] - average.rotation_rate_z, 2);
			std.user_acceleration_x += pow(row[RawColumnName::ACCLERERATION_X + 1] - average.user_acceleration_x, 2);
			std.user_acceleration_y += pow(row[RawColumnName::ACCLERERATION_Y + 1] - average.user_acceleration_y, 2);
			std.user_acceleration_z += pow(row[RawColumnName::ACCLERERATION_Z + 1] - average.user_acceleration_z, 2);
		}

		std.attitude_roll = sqrt(std.attitude_roll / total_rows);
		std.attitude_pitch = sqrt(std.attitude_pitch / total_rows);
		std.attitude_yaw = sqrt(std.attitude_yaw / total_rows);
		std.gravity_x = sqrt(std.gravity_x / total_rows);
		std.gravity_y = sqrt(std.gravity_y / total_rows);
		std.gravity_z = sqrt(std.gravity_z / total_rows);
		std.rotation_rate_x = sqrt(std.rotation_rate_x / total_rows);
		std.rotation_rate_y = sqrt(std.rotation_rate_y / total_rows);
		std.rotation_rate_z = sqrt(std.rotation_rate_z / total_rows);
		std.user_acceleration_x = sqrt(std.user_acceleration_x / total_rows);
		std.user_acceleration_y = sqrt(std.user_acceleration_y / total_rows);
		std.user_acceleration_z = sqrt(std.user_acceleration_z / total_rows);

		std::unordered_map<RawColumnName, std::pair<double, double>> columns_data = {
			{ RawColumnName::ATTITUDE_ROLL, std::pair(average.attitude_roll, std.attitude_roll) },
			{ RawColumnName::ATTITUDE_PITCH, std::pair(average.attitude_pitch, std.attitude_pitch) },
			{ RawColumnName::ATTITUDE_YAW, std::pair(average.attitude_yaw, std.attitude_yaw) },
			{ RawColumnName::GRAVITY_X, std::pair(average.gravity_x, std.gravity_x) },
			{ RawColumnName::GRAVITY_Y, std::pair(average.gravity_y, std.gravity_y) },
			{ RawColumnName::GRAVITY_Z, std::pair(average.gravity_z, std.gravity_z) },
			{ RawColumnName::ROTATION_X, std::pair(average.rotation_rate_x, std.rotation_rate_x) },
			{ RawColumnName::ROTATION_Y, std::pair(average.rotation_rate_y, std.rotation_rate_y) },
			{ RawColumnName::ROTATION_Z, std::pair(average.rotation_rate_z, std.rotation_rate_z) },
			{ RawColumnName::ACCLERERATION_X, std::pair(average.user_acceleration_x, std.user_acceleration_x) },
			{ RawColumnName::ACCLERERATION_Y, std::pair(average.user_acceleration_y, std.user_acceleration_y) },
			{ RawColumnName::ACCLERERATION_Z, std::pair(average.user_acceleration_z, std.user_acceleration_z) },
		};

		// create sets
		const std::string directory_name = current_path.parent_path().filename().string();

		const size_t directory_index = find_directory_type(directory_name);
		if (directory_index == MOVEMENT_REGEX.size())
			std::cerr << "Directory " << directory_index << " couldnt be found" << std::endl;

		const std::string filepath_name = current_path.string();
		std::smatch matches;
		if (!std::regex_search(filepath_name, matches, PERSON_FILE_REGEX))
			std::cerr << "Person file couldnt be found" << std::endl;

		const MovementType directory_type = MOVEMENT_REGEX[directory_index].second;
		const uint64_t person_id = std::stoull(matches[1].str());
		const uint64_t gender = find_gender(person_id);
#if LOGFILE
		trainset << (directory_index + 1) << DELIMITER << gender << DELIMITER << file_index;
#endif
		trainset_matrix[file_index - 1][0] = (double)(directory_index + 1);
		uint64_t trainset_lines = create_set_matrix(
			file_matrix,
			0,
			run.trainset_col,
			columns,
			run,
			columns_data,
#if LOGFILE
			trainset,
#endif
			run.trainset_col,
			trainset_matrix[file_index - 1],
			DOUBLE_INF
		);

		if (trainset_lines == run.trainset_col && file_matrix[run.trainset_col][1] != DOUBLE_INF) {
#if LOGFILE
			testset << (directory_index + 1) << DELIMITER << gender << DELIMITER << file_index;
#endif
			testset_matrix[file_index - 1][0] = (double)(directory_index + 1);
			uint64_t testset_lines = create_set_matrix(
				file_matrix,
				run.trainset_col,
				run.trainset_col + run.testset_col,
				columns,
				run,
				columns_data,
#if LOGFILE
				testset,
#endif
				run.testset_col,
				testset_matrix[file_index - 1],
				DOUBLE_INF
			);
		}

		file_index++;
	});

	// create pattern
	size_t trainset_row_index = 0;
	while (trainset_row_index < trainset_matrix.size()) {
		auto row = trainset_matrix[trainset_row_index];
		auto movement = trainset_matrix[trainset_row_index][0];

		std::vector<std::pair<double, uint64_t>> accelerations;

		while (trainset_row_index < trainset_matrix.size() && movement == trainset_matrix[trainset_row_index][0]) {
			for (size_t column_index = 1; column_index < row.size(); column_index++) {
				double acceleretion = trainset_matrix[trainset_row_index][column_index];
				size_t acceleration_index = column_index - 1;

				if (acceleration_index < accelerations.size()) {
					accelerations[acceleration_index].first += acceleretion;
					accelerations[acceleration_index].second++;
				}
				else {
					accelerations.push_back(std::pair(acceleretion, 1));
				}
			}
			trainset_row_index++;
		}

#if LOGFILE
		pattern << movement;
#endif
		pattern_matrix[movement - 1][0] = (double)(movement);
		for (size_t acc_index = 0; acc_index < accelerations.size(); acc_index++) {
			double cummulation = accelerations[acc_index].first;
			uint64_t stack_count = accelerations[acc_index].second;

			double value = cummulation / stack_count;
#if LOGFILE
			pattern << DELIMITER << value;
#endif
			pattern_matrix[movement - 1][acc_index + 1] = (double)(value);
		}
#if LOGFILE
		pattern << std::endl;
#endif

	}

	// create evaluation
	for (size_t testset_row_index = 0; testset_row_index < 360; testset_row_index++) {
		auto is_sensitive = sensitives_files.find(testset_row_index) != sensitives_files.end();
		int64_t testset_lines = is_sensitive ? (int64_t)(sensitives_files.at(testset_row_index) - run.trainset_col) : run.testset_col;
		if (testset_lines > 0) {
			double distance_min = DOUBLE_INF;
			uint64_t movement_min = static_cast<uint64_t>(MovementType::SIZE);
			for (size_t pattern_row_index = 0; pattern_row_index < 6; pattern_row_index++) {
				double total = 0;
				for (size_t min_column_index = 1; min_column_index < std::min(run.trainset_col, run.testset_col); min_column_index++) {
					auto testset_value = testset_matrix[testset_row_index][min_column_index];
					auto pattern_value = pattern_matrix[pattern_row_index][min_column_index];
					total += std::pow(testset_value - pattern_value, 2);
				}
				double distance = std::sqrt(total);
				if (distance < distance_min) {
					distance_min = distance;
					movement_min = pattern_row_index;
				}
			}
			const uint64_t movement_index = (uint64_t)(testset_matrix[testset_row_index][0] - 1);
			evaluation_matrix[movement_index][movement_min]++;
		}
	}

#if EVALUTIONFILE
	for (size_t row_index = 0; row_index < 6; row_index++) {
		evaluation << (row_index + 1) << DELIMITER;
		for (size_t column_index = 0; column_index < 6; column_index++) {
			auto value = evaluation_matrix[row_index][column_index];
			evaluation << value << DELIMITER;
		}
		evaluation << std::endl;
	}
#endif
	return evaluation_matrix;
}