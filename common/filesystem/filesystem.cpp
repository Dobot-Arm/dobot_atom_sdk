#include "filesystem.hpp"
#include <sstream>
namespace Atom
{
    namespace Common
    {
        std::vector<std::vector<double>> FileSystemHelper::readCSV(const std::string &filePath)
        {
            std::vector<std::vector<double>> data;
            std::ifstream file(filePath);
            std::string line;
            if (!file.is_open()) {
                std::cerr << "ERROR: file open failed: " << filePath << std::endl;
                return data;
            }
            while (std::getline(file, line)) {
                std::stringstream ss(line);
                std::string value;
                std::vector<double> row;
                while (std::getline(ss, value, ',')) { row.push_back(std::stod(value)); }
                data.push_back(row);
            }
            return data;
        }

        // Extract non-zero elements by known indices
        std::vector<std::vector<double>> FileSystemHelper::extractElements(const std::vector<std::vector<double>> &data, const std::vector<size_t> &selected_indices)
        {
            std::vector<std::vector<double>> result;
            for (const auto &row : data) {
                std::vector<double> selected;
                for (size_t idx : selected_indices) {
                    if (idx < row.size()) { selected.push_back(row[idx]); }
                }
                result.push_back(selected);
            }
            return result;
        }

    }  // namespace Common
}  // namespace Atom