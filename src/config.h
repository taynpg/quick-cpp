#pragma once

#include <QDir>
#include <QString>
#include <string>

struct SConfig {
    std::string config_path{};
    int     font_size{14};
    std::string font_family{};
    std::string query_driver{};
    std::string qt_dir{};
    std::string boost_dir{};
    std::string project_type{};
    std::string compiler{};
    std::string type{};
};

class ConfigOpr {
public:
    ConfigOpr() = default;
    ~ConfigOpr() = default;

public:
    static void read_config(SConfig& config);
    static void set_default(const QString& config_path);
};
