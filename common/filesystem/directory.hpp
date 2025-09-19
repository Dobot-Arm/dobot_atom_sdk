#ifndef __DIRECTORY_HPP__
#define __DIRECTORY_HPP__
#include "sdk/common/decl.hpp"

namespace Atom
{
    namespace Common
    {
        class Directory
        {
        public:
            static std::string getProjectPath();
            static std::string getProjectPath(const std::string &relative_path);

        private:
            Directory() = delete;
            Directory(const Directory &) = delete;
            Directory &operator=(const Directory &) = delete;
        };

    }  // namespace Common
}  // namespace Atom

#endif  // __DIRECTORY_HPP