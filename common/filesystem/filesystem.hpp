#ifndef __FILESYSTEM_HPP__
#define __FILESYSTEM_HPP__
#include "sdk/common/decl.hpp"
namespace Atom
{
    namespace Common
    {
        class FileSystemHelper
        {
        public:
            static std::vector<std::vector<double>> readCSV(const std::string &filePath);
            static std::vector<std::vector<double>> extractElements(const std::vector<std::vector<double>> &data, const std::vector<size_t> &selected_indices);
            static std::vector<std::vector<double>> readCSVSelected(const std::string &filePath, const std::vector<size_t> &selected_indices);  // TODO

        private:
            FileSystemHelper() = delete;
            FileSystemHelper(const FileSystemHelper &) = delete;
            FileSystemHelper &operator=(const FileSystemHelper &) = delete;
        };

    }  // namespace Common
}  // namespace Atom

#endif  // __FILESYSTEM_HPP__