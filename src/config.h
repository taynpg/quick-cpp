#pragma once

#include <string>
#include <boost/filesystem.hpp>

struct SConfig {
    std::string config_path{};
    int         font_size{14};
    std::string font_family{};
    std::string query_driver{};
    std::string qt_dir{};
    std::string boost_dir{};
    std::string project_type{};
    std::string compiler{};
    std::string type{};
    std::string project_name{};
    std::string project_dir{};
};

class ConfigOpr {
public:
    ConfigOpr() = default;
    ~ConfigOpr() = default;

public:
    void read_config(SConfig& config);
    void set_default(const std::string& config_path);
private:
    std::string get_home();
};
