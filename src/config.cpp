#include "config.h"

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/ini_parser.hpp>

#if defined(__clang__) && defined(__APPLE__)
//#include <mach-o/dyld.h>
#elif defined(_MSC_VER) || defined(__MINGW32__) || defined(__MINGW64__)
//#include <windows.h>
#else
#include <unistd.h>
#include <sys/types.h>
#include <pwd.h>
#endif

constexpr auto DEFAULT_PROJECT_TYPE = "DefaultProjectType";
constexpr auto QT_DIR = "QtDir";
constexpr auto BOOST_DIR = "BoostDir";
constexpr auto COMPILER = "Compiler";
constexpr auto FONT_SIZE = "FontSize";
constexpr auto FONT_FAMILY = "FontFamily";
constexpr auto QUERY_DRIVER = "QueryDriver";

namespace fs = boost::filesystem;

std::string ConfigOpr::get_home()
{
#if defined(__clang__) && defined(__APPLE__)
    return "";
#elif defined(_MSC_VER) || defined(__MINGW32__) || defined(__MINGW64__)
    return std::getenv("USERPROFILE");
#else
    const char *homedir{};
    if ((homedir = getenv("HOME")) == NULL) {
        homedir = getpwuid(getuid())->pw_dir;
    }
    return std::string(homedir);
#endif
}

void ConfigOpr::read_config(SConfig& config)
{
    std::string home = get_home();
    fs::path config_dir = fs::path(home).append(".config").append("quick-cpp");
    fs::path logfile = fs::path(config_dir).append("config.ini");

    if (!fs::exists(config_dir)) {
        fs::create_directory(config_dir);
    }
    if (!fs::exists(logfile)) {
        set_default(logfile.string());
        return;
    }
    boost::property_tree::ptree pt, node;
    boost::property_tree::ini_parser::read_ini(logfile.string(), pt);
    node = pt.get_child("Basic");

    config.type = node.get<std::string>(DEFAULT_PROJECT_TYPE, "common");
    config.boost_dir = node.get<std::string>(BOOST_DIR, R"(C:\Bin\Boost)");
    config.font_size = node.get<int>(FONT_SIZE, 15);
    config.font_family = node.get<std::string>(FONT_FAMILY, "FiraCode Nerd Font Mono");
    config.query_driver = node.get<std::string>(QUERY_DRIVER, "/usr/bin/g++");
    config.compiler = node.get<std::string>(COMPILER, "msvc");
}

void ConfigOpr::set_default(const std::string& config_path)
{
    boost::property_tree::ptree pt, node;

    node.put(DEFAULT_PROJECT_TYPE, "common");
    node.put(BOOST_DIR, R"(C:\Bin\Boost)");
    node.put(FONT_SIZE, 15);
    node.put(FONT_FAMILY, "FiraCode Nerd Font Mono");
    node.put(QUERY_DRIVER, "/usr/bin/g++");
    node.put(COMPILER, "msvc");
    pt.put_child("Basic", node);

    boost::property_tree::ini_parser::write_ini(config_path, pt);
}
