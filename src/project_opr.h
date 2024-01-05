#pragma once

#include <string>

#include "config.h"

class ProjectOpr {
public:
    ProjectOpr() = default;
    virtual ~ProjectOpr() = default;

public:
    virtual bool Run(const SConfig& config);
    std::string  get_last_error();

protected:
    static std::string read_txt(const std::string& path);
    static bool  write_txt(const std::string& path, const std::string& content);
    virtual void handle_setting() = 0;
    virtual void handle_main() = 0;
    virtual void handle_cmakelist() = 0;
    virtual void clear();

protected:
    SConfig config_{};

    std::string source_main{};
    std::string des_main{};
    std::string source_cmakelist{};
    std::string des_cmakelist{};

    std::string source_setting{};
    std::string vscode_dir{};
    std::string des_setting{};
    std::string error_{};
    std::string purpose_dir_{};
};

class ProjectConsoleOpr : public ProjectOpr {
public:
    ProjectConsoleOpr() = default;
    virtual ~ProjectConsoleOpr() = default;

public:
    bool Run(const SConfig& config) override;
};
