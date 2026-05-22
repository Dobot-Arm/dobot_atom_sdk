#include "filesystem.hpp"
#include <sstream>
#include <fstream>
#include <filesystem>
#include <algorithm>
#include <stdexcept>
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
            
            int line_number = 0;
            while (std::getline(file, line)) {
                line_number++;
                if (line.empty() || line.find_first_not_of(" \t\r\n") == std::string::npos) {
                    continue;
                }
                
                std::stringstream ss(line);
                std::string value;
                std::vector<double> row;
                bool row_valid = true;
                
                while (std::getline(ss, value, ',')) {
                    size_t first = value.find_first_not_of(" \t\r\n");
                    if (first == std::string::npos) {
                        std::cerr << "WARNING: Empty cell found in file " << filePath 
                                  << " at line " << line_number << ". Skipping this line to maintain data continuity." << std::endl;
                        row_valid = false;
                        break;
                    }
                    size_t last = value.find_last_not_of(" \t\r\n");
                    value = value.substr(first, last - first + 1);
                    
                    try {
                        size_t pos = 0;
                        double num = std::stod(value, &pos);
                        if (pos == value.length()) {
                            row.push_back(num);
                        } else {
                            std::cerr << "WARNING: Invalid number format in file " << filePath 
                                      << " at line " << line_number << ", value: \"" << value << "\". Skipping this line." << std::endl;
                            row_valid = false;
                            break;
                        }
                    } catch (const std::invalid_argument &e) {
                        std::cerr << "WARNING: Invalid argument in file " << filePath 
                                  << " at line " << line_number << ", value: \"" << value << "\". Skipping this line." << std::endl;
                        row_valid = false;
                        break;
                    } catch (const std::out_of_range &e) {
                        std::cerr << "WARNING: Number out of range in file " << filePath 
                                  << " at line " << line_number << ", value: \"" << value << "\". Skipping this line." << std::endl;
                        row_valid = false;
                        break;
                    }
                }
                
                if (row_valid && !row.empty()) {
                    data.push_back(row);
                }
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

        // Scan directory for CSV files
        std::vector<std::string> FileSystemHelper::scanCSVFiles(const std::string &directory_path)
        {
            std::vector<std::string> csv_files;
            try {
                if (!std::filesystem::exists(directory_path)) {
                    std::cerr << "ERROR: Directory does not exist: " << directory_path << std::endl;
                    return csv_files;
                }

                for (const auto &entry : std::filesystem::directory_iterator(directory_path)) {
                    if (entry.is_regular_file()) {
                        std::string filename = entry.path().filename().string();
                        if (filename.length() >= 4) {
                            std::string ext = filename.substr(filename.length() - 4);
                            std::transform(ext.begin(), ext.end(), ext.begin(), ::tolower);
                            if (ext == ".csv") {
                                csv_files.push_back(filename);
                            }
                        }
                    }
                }
                std::sort(csv_files.begin(), csv_files.end());
            } catch (const std::filesystem::filesystem_error &e) {
                std::cerr << "ERROR: Failed to scan directory " << directory_path << ": " << e.what() << std::endl;
            }
            return csv_files;
        }

    }  // namespace Common
}  // namespace Atom