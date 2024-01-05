#pragma once

#include <string>

#include "config.h"


class ProjectOpr {
public:
    ProjectOpr() = default;
    ~ProjectOpr() = default;

public:
    bool Run(const SConfig& config);
    std::string get_last_error();

private:
    std::string read_txt(const std::string& path);
    bool        write_txt(const std::string& path, const std::string& content);
    void        handle_setting();
    void        handle_main();
    void        handle_cmakelist();
    void        clear();
private:
    SConfig config_{};

    std::string source_main{};
    std::string des_main{};
    std::string source_cmakelist{};
    std::string des_cmakelist{};

    std::string source_setting{};
    std::string vscode_dir{};
    std::string des_setting{};
    std::string error_{};
};
