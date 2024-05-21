#include "Global.hpp"

#define EXTRA_DEBUG 0

[[nodiscard]]
uint64_t create_set_dyn(
	const RunParameter& parameter,
	const GlobalState& global,
	std::fstream& current_file,
	std::fstream& output_file,
	const uint64_t line_count
) {
	uint64_t lines_explored = 0;

	std::string line;
	while (lines_explored < line_count && std::getline(current_file, line)) {		
		bool is_extreme = false;
		double total = 0;

		char delimiter;
		double value;
		uint64_t id;

		std::istringstream line_iss(line);
		line_iss >> id >> delimiter;

		std::string got;
		for (size_t column_index = 0; !line_iss.eof(); column_index++) {
			line_iss >> value >> delimiter;
			
			RawColumnName column_name = static_cast<RawColumnName>(column_index);

			auto egaler = [&](RawColumnName name) { return name == column_name; };

			auto found_col = std::find_if(parameter.extract_col.begin(), parameter.extract_col.end(), egaler);
			if (found_col != parameter.extract_col.end()) {
				auto found_name = global.columns_data.find(column_name);
				if (found_name == global.columns_data.end()) {
					std::cerr << "Couldnt find column data" << std::endl;
				}
				auto column_data = global.columns_data.at(column_name);
				is_extreme |= parameter.extreme(value, column_data.first, column_data.second);

				total += pow(value, 2);

				got += RAW_COLUMN_MAP[column_name] + ": " + std::to_string(value) + " | ";
			}
		}

		if (!is_extreme) {
			double acceleration = sqrt(total);

			output_file << DELIMITER << acceleration;
#if EXTRA_DEBUG
			std::cout << std::to_string(is_extreme) + " - " + got << std::endl;
#endif // EXTRA_DEBUG

		}
		lines_explored++;
	}

	output_file << std::endl;

	return lines_explored;
}