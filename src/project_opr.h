#pragma once

#include <string>
#include <QString>
#include <QFile>
#include "config.h"

std::string ss(const QString& str);
QString     qs(const std::string& str);
void MCopy(const QString& from, const QString& des);

class ProjectOpr {
public:
    ProjectOpr() = default;
    virtual ~ProjectOpr() = default;

public:
    virtual bool Run(const SConfig& config);
    std::string  get_last_error();

protected:
    std::string  read_txt(const std::string& path);
    bool         write_txt(const std::string& path, const std::string& content);
    virtual void handle_setting() = 0;
    virtual void handle_main() = 0;
    virtual void handle_cmakelist() = 0;
    virtual void clear();
    virtual void basic_replace();

protected:
    SConfig     config_{};
    std::string settings_{};

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
    void handle_setting() override;
    void handle_main() override;
    void handle_cmakelist() override;
};

class ProjectQtOpr : public ProjectConsoleOpr {
public:
    ProjectQtOpr() = default;
    virtual ~ProjectQtOpr() = default;

public:
    bool Run(const SConfig& config) override;
    void handle_setting() override;
    void handle_main() override;
    void handle_cmakelist() override;
};

class ProjectBoostOpr : public ProjectConsoleOpr {
public:
    ProjectBoostOpr() = default;
    virtual ~ProjectBoostOpr() override = default;

public:
    bool Run(const SConfig& config) override;
    void handle_setting() override;
    void handle_cmakelist() override;
};
