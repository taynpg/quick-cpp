#include "project_opr.h"

#include <QFile>
#include <boost/algorithm/string.hpp>
#include <boost/filesystem.hpp>
#include <fstream>
#include <iostream>


namespace fs = boost::filesystem;

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
    des_cmakelist = fs::path(purpose_dir_).append("CMakeLists.txt").string();
    des_setting = fs::path(vscode_dir).append("settings.json").string();
    vscode_dir = fs::path(purpose_dir_).append(".vscode").string();

    if (config_.project_type == "qt") {
        QFile::copy(
            "://template/qt5.natvis",
            QString::fromLocal8Bit(
                fs::path(vscode_dir).append("qt5.natvis").string().c_str()));
        QFile::copy(
            "://template/qt6.natvis",
            QString::fromLocal8Bit(
                fs::path(vscode_dir).append("qt6.natvis").string().c_str()));
    }

    handle_setting();
    handle_main();
    handle_cmakelist();
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

void ProjectOpr::handle_setting()
{
    std::string setting = read_txt(source_setting);
    std::string newa{};
    std::string newb{};

    if (config_.project_type == "console") {
        newa = "";
        newb = "";
    }
    if (config_.project_type == "qt") {
        newa =
            ",\n        \"visualizerFile\": "
            "\"${workspaceRoot}/.vscode/qt6.natvis\"";
        newb =
            "\n    \"cmake.environment\": {\n        \"PATH\": \"${env:PATH};";
        newb.append(config_.qt_dir + "/bin\"\n    },");
    }
    boost::replace_all(setting, "replaceA", newa);
    boost::replace_all(setting, "replaceB", newb);
    boost::replace_all(setting, "replaceC", std::to_string(config_.font_size));
    boost::replace_all(setting, "replaceD", config_.font_family);

    if (setting.empty()) {
        return;
    }
    write_txt(des_setting, setting);
}

void ProjectOpr::handle_main()
{
    if (config_.project_type == "console" || config_.project_type == "boost") {
        QFile::copy(QString::fromLocal8Bit(source_main.c_str()),
                    QString::fromLocal8Bit(des_main.c_str()));
    }
    if (config_.project_type == "qt") {
        fs::path dir = fs::path(des_main).parent_path();
        std::string m(fs::path(dir).append("main.cpp").string());
        std::string mwc(fs::path(dir).append("MainWidget.cpp").string());
        std::string mwh(fs::path(dir).append("MainWidget.h").string());
        std::string mu(fs::path(dir).append("MainWidget.ui").string());
    }
}

void ProjectOpr::handle_cmakelist()
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
    // std::ifstream in(path, std::ios::in);
    // if (!in.is_open()) {
    //     std::cout << "读取：" << path << "失败!\n";
    //     return result;
    // }
    // std::stringstream buffer{};
    // buffer << in.rdbuf();
    // in.close();
    // result = buffer.str();
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

    fs::create_directory(purpose_dir_);
    fs::create_directory(vscode_dir);
}