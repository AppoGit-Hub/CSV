// frederic.clerin@henallux.be

#include <thread>
#include <span>

#include "Global.h"
#include "InputStream.h"
#include "OutputStream.h"

static std::unique_ptr<fs::path> get_subject_filepath() noexcept {
	return std::make_unique<fs::path>(fs::path{ BASE_FOLDER } / SUBJECT_FILENAME);
}

static std::unique_ptr<fs::path> get_data_filepath() noexcept {
	return std::make_unique<fs::path>(fs::path{ BASE_FOLDER } / DATA_FOLDER);
}

static const MovementType& find_directory_type(const fs::path& directory) noexcept {
	const auto& directory_name = directory.filename();
	for (const auto& pair : MOVEMENT_REGEX) {
		if (std::regex_match(directory_name.string(), pair.first)) {
			return pair.second;
		}
	}
}

static std::string find_person_id(const fs::path& filepath) noexcept {
	const auto& filename = filepath.filename().string();
	std::smatch matches;
	if (std::regex_search(filename, matches, PERSON_FILE_REGEX)) {
		if (matches.size() > 1) {
			return matches[1].str();
		}
	}
}

static void explore_directory(const fs::path& directory, std::vector<fs::path>& files) noexcept {
	for (const auto& entry : fs::directory_iterator(directory)) {
		const auto& current_path = entry.path();
		if (fs::is_regular_file(entry.path())) {
			files.push_back(current_path);
		}
		else if (fs::is_directory(entry.path())) {
			explore_directory(entry.path(), files);
		}
	}
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

int other() {
	const auto& data_ptr = get_data_filepath();
	const auto& data_filepath = *data_ptr.get();

	const auto& subject_ptr = get_subject_filepath();
	const auto& subject_filepath = *subject_ptr.get();

	std::vector<fs::path> all_files;
	explore_directory(data_filepath, all_files);

	const auto& subject_handler = std::make_shared<InputStream>(subject_filepath, ',', true);

	const auto& output_handler = std::make_shared<OutputStream>(
		fs::path("output.csv"), 
		std::vector<std::string> { "mouvement", "genre", "index", "acceleration" }
	);

	std::cout << extract_string("one;two;three;four;five;six", ';', 2) << std::endl;
	
	
	//launch(all_files, subject_handler, output_handler, 0);

	return EXIT_SUCCESS;
}