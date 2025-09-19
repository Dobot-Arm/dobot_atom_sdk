#include "directory.hpp"
#define STR_HELPER(x) #x
#define STR(x) STR_HELPER(x)
namespace Atom
{
    namespace Common
    {
        std::string Directory::getProjectPath() { return std::string(STR(PROJECT_DIR)); }
        std::string Directory::getProjectPath(const std::string &relative_path) { return std::string(STR(PROJECT_DIR)) + relative_path; }
    }  // namespace Common
}  // namespace Atom