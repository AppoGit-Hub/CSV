#include "Global.hpp"

void filter_justone(
	const std::vector<double>& values,
	ExtremeFunction extreme_func,
	std::fstream& output_file
) {

}

[[nodiscard]]
uint64_t create_set_dyn(
	const RunParameter& parameter,
	const GlobalState& global,
	std::fstream& current_file,
	std::fstream& output_file,
	const uint64_t line_count,
) {
	uint64_t lines_explored = 0;

	std::string line;
	while (lines_explored < line_count && std::getline(current_file, line)) {		
		bool is_extreme;

		char delimiter;
		double value;
		uint64_t id;

		std::istringstream line_iss(line);
		line_iss >> id >> delimiter;

		for (size_t column_index = 0; !line_iss.eof(); column_index++) {
			line_iss >> value >> delimiter;
			
			RawColumnName column_name = static_cast<RawColumnName>(column_index);

			auto found_col = std::find_if(parameter.extract_col.begin(), parameter.extract_col.end(), column_name);
			if (found_col != parameter.extract_col.end()) {
				auto found_name = global.columns_data.find(column_name);
				if (found_name == global.columns_data.end()) {
					std::cerr << "Couldnt find column data" << std::endl;
				}



				is_extreme |= parameter.extreme(value, 0, 0);
			}
		}

		lines_explored++;
	}
	output_file << std::endl;

	return lines_explored;
}