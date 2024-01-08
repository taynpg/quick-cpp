#include "project_opr.h"

#include <QFile>
#include <boost/algorithm/string.hpp>
#include <boost/filesystem.hpp>
#include <fstream>
#include <iostream>

namespace fs = boost::filesystem;

void MCopy(const QString& from, const QString& des)
{
    QFile::copy(from, des);
    QFile file(des);
    file.setPermissions(QFile::ReadOwner | QFile::WriteOwner);
}

bool ProjectOpr::Run(const SConfig& config)
{
    config_ = config;
    purpose_dir_ =
        fs::path(config_.project_dir).append(config.project_name).string();

    if (fs::exists(purpose_dir_)) {
        error_ = u8"文件夹已存在。";
        return false;
    }
    clear();
    vscode_dir = fs::path(purpose_dir_).append(".vscode").string();
    des_setting = fs::path(vscode_dir).append("settings.json").string();
    des_cmakelist = fs::path(purpose_dir_).append("CMakeLists.txt").string();

    fs::create_directory(purpose_dir_);
    fs::create_directory(vscode_dir);
    return true;
}

void ProjectOpr::clear()
{
    source_main.clear();
    des_main.clear();
    source_cmakelist.clear();
    des_cmakelist.clear();
    source_setting.clear();
    vscode_dir.clear();
    des_setting.clear();
}

void ProjectQtOpr::handle_setting()
{
    settings_ = read_txt(source_setting);
    std::string newa{};
    std::string newb{};

    newa = ",\n        \"visualizerFile\": "
           "\"${workspaceRoot}/.vscode/qt6.natvis\"";

    if (!config_.is_static) {
        newb = "\n    \"cmake.environment\": {\n        \"PATH\": \"${env:PATH};";
        newb.append(config_.qt_dir + "/bin\"\n    },");
    }

    boost::replace_all(settings_, "replaceA", newa);
    boost::replace_all(settings_, "replaceB", newb);
    ProjectOpr::basic_replace();

    MCopy(
        "://template/qt5.natvis",
        QString::fromLocal8Bit(
            fs::path(vscode_dir).append("qt5.natvis").string().c_str()));
    MCopy(
        "://template/qt6.natvis",
        QString::fromLocal8Bit(
            fs::path(vscode_dir).append("qt6.natvis").string().c_str()));

    if (settings_.empty()) {
        return;
    }
    write_txt(des_setting, settings_);
}

void ProjectBoostOpr::handle_setting()
{
    settings_ = read_txt(source_setting);
    std::string newa{};
    std::string newb{};

    if (!config_.is_static) {
        newb = "\n    \"cmake.environment\": {\n        \"PATH\": \"${env:PATH};";
        newb.append(config_.boost_dir + "/lib\"\n    },");
    }

    boost::replace_all(settings_, "replaceA", newa);
    boost::replace_all(settings_, "replaceB", newb);
    ProjectOpr::basic_replace();

    if (settings_.empty()) {
        return;
    }
    write_txt(des_setting, settings_);
}

void ProjectConsoleOpr::handle_setting()
{
    settings_ = read_txt(source_setting);
    std::string newa{};
    std::string newb{};

    boost::replace_all(settings_, "replaceA", newa);
    boost::replace_all(settings_, "replaceB", newb);
    ProjectOpr::basic_replace();

    if (settings_.empty()) {
        return;
    }
    write_txt(des_setting, settings_);
}

void ProjectOpr::basic_replace()
{
    if (settings_.empty()) {
        return;
    }
    boost::replace_all(settings_, "replaceC",
                       std::to_string(config_.font_size));
    boost::replace_all(settings_, "replaceD", config_.font_family);
    if (config_.query_driver.empty()) {
        config_.query_driver = "/usr/bin/g++";
    }
    boost::replace_all(settings_, "replaceE", config_.query_driver);
}

void ProjectQtOpr::handle_main()
{
    fs::path    dir = fs::path(des_main).parent_path();
    std::string mwc(fs::path(dir).append("MainWidget.cpp").string());
    std::string mwh(fs::path(dir).append("MainWidget.h").string());
    std::string mu(fs::path(dir).append("MainWidget.ui").string());

    MCopy("://template/qt/main.cpp", qs(des_main));
    MCopy("://template/qt/MainWidget.cpp", qs(mwc));
    MCopy("://template/qt/MainWidget.h", qs(mwh));
    MCopy("://template/qt/MainWidget.ui", qs(mu));
}

void ProjectConsoleOpr::handle_main()
{
    MCopy(QString::fromLocal8Bit(source_main.c_str()),
                QString::fromLocal8Bit(des_main.c_str()));
}

void ProjectQtOpr::handle_cmakelist()
{
    std::string cmakelist = read_txt(source_cmakelist);
    boost::replace_all(cmakelist, "untitled1", config_.project_name);
    boost::replace_all(cmakelist, "replaceA", config_.qt_dir);

    if (cmakelist.empty()) {
        return;
    }
    write_txt(des_cmakelist, cmakelist);
}

void ProjectBoostOpr::handle_cmakelist()
{
    std::string cmakelist = read_txt(source_cmakelist);
    boost::replace_all(cmakelist, "replaceB", config_.project_name);
    boost::replace_all(cmakelist, "replaceA", config_.boost_dir);

    if (config_.is_static) {
        boost::replace_all(cmakelist, "replaceC", "ON");
    }
    else {
        boost::replace_all(cmakelist, "replaceC", "OFF");
    }

    if (cmakelist.empty()) {
        return;
    }
    write_txt(des_cmakelist, cmakelist);
}

void ProjectConsoleOpr::handle_cmakelist()
{
    std::string cmakelist = read_txt(source_cmakelist);
    boost::replace_all(cmakelist, "replace", config_.project_name);
    if (cmakelist.empty()) {
        return;
    }
    write_txt(des_cmakelist, cmakelist);
}

std::string ProjectOpr::get_last_error() { return error_; }

std::string ProjectOpr::read_txt(const std::string& path)
{
    std::string result{};

    QFile file(QString::fromStdString(path));
    if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QString content = file.readAll();
        file.close();
        result.append(content.toLocal8Bit().constData());
    } else {
        std::cout << u8"读取：" << path << u8"失败!";
    }
    return result;
}

bool ProjectOpr::write_txt(const std::string& path, const std::string& content)
{
    std::ofstream out(path, std::ios::out);
    if (!out.is_open()) {
        error_ = u8"打开：" + path + u8"失败!";
        return false;
    }
    out << content;
    out.close();
    return true;
}

bool ProjectBoostOpr::Run(const SConfig& config)
{
    return ProjectConsoleOpr::Run(config);
}

bool ProjectQtOpr::Run(const SConfig& config)
{
    return ProjectConsoleOpr::Run(config);
}

bool ProjectConsoleOpr::Run(const SConfig& config)
{
    if (!ProjectOpr::Run(config)) {
        return false;
    }

    source_main = "://template/" + config_.project_type + "/main.cpp";
    des_main = fs::path(purpose_dir_).append("main.cpp").string();
    source_cmakelist =
        "://template/" + config_.project_type + "/CMakeLists.txt";
    source_setting = "://template/settings-" + config_.compiler + ".json";

    handle_main();
    handle_setting();
    handle_cmakelist();

    if (config_.is_export_clangd_ini) {
        MCopy("://template/.clangd", qs(fs::path(purpose_dir_).append(".clangd").string()));
        MCopy("://template/.clang-format", qs(fs::path(purpose_dir_).append(".clang-format").string()));
    }

    return true;
}
