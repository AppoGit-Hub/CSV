// frederic.clerin@henallux.be
#include "Global.h"

static fs::path get_subject_filepath() noexcept {
	return fs::path{ BASE_FOLDER } / SUBJECT_FILENAME;
}

static fs::path get_data_filepath() noexcept {
	return fs::path{ BASE_FOLDER } / DATA_FOLDER;
}

static const MovementType& find_directory_type(const fs::path& directory) noexcept {
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

	char delimiter = ',';
	
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

/*
static void launch(
	const std::vector<fs::path> files,
	const std::shared_ptr<InputStream>& subject_handler,
	const std::shared_ptr<OutputStream>& output_handler,
	const uint64_t& bundle_index
) {
	for (size_t index = 0; index < files.size(); index++) {
		const fs::path& filepath = files[index];
		
		std::cout << filepath << std::endl;

		const auto& directory_type = find_directory_type(filepath.parent_path());
		const auto& person_id = find_person_id(filepath);

		const auto& file_handler = std::make_unique<InputStream>(filepath, ',', false);

		for (size_t row_index = 0; row_index < file_handler->get_depth(); row_index++) {
			const double& acceleration_x = std::stod(file_handler->get_at("userAcceleration.x", index));
			const double& acceleration_y = std::stod(file_handler->get_at("userAcceleration.y", index));
			const double& acceleration_z = std::stod(file_handler->get_at("userAcceleration.z", index));
			const double& accleration = std::sqrt(std::pow(acceleration_x, 2) + std::pow(acceleration_y, 2) + std::pow(acceleration_z, 2));

			const auto& gender = subject_handler->get_column_where("code", "gender", person_id);

			output_handler->add_line({
				std::to_string(static_cast<uint64_t>(directory_type)),
				gender,
				std::to_string(bundle_index + index),
				std::to_string(accleration)
			});
		}
	}
}
*/

static void go(
	uint64_t& file_index,
	const fs::path& directory, 
	std::ofstream& trainset,
	std::ofstream& testset,
	std::ifstream& subjects) {
	for (const auto& entry : fs::directory_iterator(directory)) {
		const auto& current_path = entry.path();
		if (fs::is_regular_file(current_path)) {
			char delimiter = ',';

			const auto& directory_type = find_directory_type(current_path.parent_path());
			const uint64_t person_id = find_person_id(current_path);
			const uint64_t gender = find_gender(person_id, subjects);

			std::ifstream file(current_path);

			std::string header;
			std::getline(file, header);

			uint64_t max_lines = 0;
			uint64_t min_lines = UINT64_MAX;

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

			trainset << static_cast<uint64_t>(directory_type) << delimiter << gender << delimiter << file_index;

			std::string line;
			uint64_t traint_index;
			for (traint_index = 0; traint_index < 600 && std::getline(file, line); traint_index++) {
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
				trainset << delimiter << acceleration;
			}
			trainset << '\n';

			if (traint_index == 600) {
				testset << static_cast<uint64_t>(directory_type) << delimiter << gender << delimiter << file_index;
				for (uint64_t test_index = 600; test_index < 660 && std::getline(file, line); test_index++) {
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
					testset << delimiter << acceleration;
				}
				testset << '\n';
			}

			file_index++;
		}
		else if (fs::is_directory(current_path)) {
			go(file_index, current_path, trainset, testset, subjects);
		}
	}
}

int main() {
	const auto& data_filepath = get_data_filepath();
	const auto& subject_filepath = get_subject_filepath();
	
	std::ofstream trainset_file("trainset.csv", std::ios::app);
	std::ofstream testset_file("testset.csv", std::ios::app);
	std::ifstream subjects_file(subject_filepath);

	trainset_file << "Mouvement, Gender, Index";
	for (uint64_t index = 0; index < 600; index++) {
		trainset_file << ", Vacc";
	}
	trainset_file << "\n";

	testset_file << "Mouvement, Gender, Index";
	for (uint64_t index = 0; index < 60; index++) {
		testset_file << ", Vacc";
	}
	testset_file << "\n";

	uint64_t file_index = 1;
	go(file_index, data_filepath, trainset_file, testset_file, subjects_file);

	return EXIT_SUCCESS;
}