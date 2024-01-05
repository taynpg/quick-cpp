#pragma once

#include <string>
#include <boost/filesystem.hpp>

namespace fs = boost::filesystem;

struct SConfig {
    int         font_size{14};
    std::string font_family{};
    std::string query_driver{};
    std::string qt_dir{};
    std::string boost_dir{};
    std::string project_type{};
    std::string compiler{};
    std::string project_name{};
    std::string project_dir{};
};

class ConfigOpr {
public:
    ConfigOpr() = default;
    ~ConfigOpr() = default;

public:
    void read_config(SConfig& config);
    void save_config(const SConfig& config);
private:
    std::string get_home();
    fs::path ini_path_{};
};
