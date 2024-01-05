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
constexpr auto PROJECT_DIR = "ProjectDir";

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
    ini_path_ = fs::path(config_dir).append("config.ini");

    if (!fs::exists(config_dir)) {
        fs::create_directory(config_dir);
    }
    if (!fs::exists(ini_path_)) {
        return;
    }
    boost::property_tree::ptree pt, node;
    boost::property_tree::ini_parser::read_ini(ini_path_.string(), pt);
    node = pt.get_child("Basic");

    config.project_type = node.get<std::string>(DEFAULT_PROJECT_TYPE, "console");
    config.boost_dir = node.get<std::string>(BOOST_DIR, R"(C:/Bin/Boost)");
    config.font_size = node.get<int>(FONT_SIZE, 15);
    config.font_family = node.get<std::string>(FONT_FAMILY, "FiraCode Nerd Font Mono");
    config.query_driver = node.get<std::string>(QUERY_DRIVER, "/usr/bin/g++");
    config.compiler = node.get<std::string>(COMPILER, "msvc");
    config.project_dir = node.get<std::string>(PROJECT_DIR, "D:/");
}

void ConfigOpr::save_config(const SConfig& config)
{
    boost::property_tree::ptree pt, node;

    if (fs::exists(ini_path_)) {
        boost::property_tree::ini_parser::read_ini(ini_path_.string(), pt);
        node = pt.get_child("Basic");
    }
    node.put(DEFAULT_PROJECT_TYPE, config.project_type);
    node.put(BOOST_DIR, config.boost_dir);
    node.put(QT_DIR, config.qt_dir);
    node.put(FONT_SIZE, config.font_size);
    node.put(FONT_FAMILY, config.font_family);
    node.put(QUERY_DRIVER, config.query_driver);
    node.put(COMPILER, config.compiler);
    node.put(PROJECT_DIR, config.project_dir);
    pt.put_child("Basic", node);
    boost::property_tree::ini_parser::write_ini(ini_path_.string(), pt);
}
