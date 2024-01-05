#include "project_opr.h"

#include <boost/algorithm/string.hpp>
#include <boost/filesystem.hpp>
#include <fstream>
#include <iostream>
#include <QFile>

namespace fs = boost::filesystem;

bool ProjectOpr::Run(const SConfig& config)
{
    config_ = config;
    
    std::string purpose_dir = fs::path(config_.project_dir).append(config.project_name).string();
    if (fs::exists(purpose_dir)) {
        std::cout << "文件夹已存在。" << std::endl;
        return false;
    }
    clear();
    source_main = "://template/" + config_.project_type + "/main.cpp";
    des_main = fs::path(purpose_dir).append("main.cpp").string();
    source_cmakelist = "://template/" + config_.project_type + "/CMakeLists.txt";
    des_cmakelist =
        fs::path(purpose_dir).append("CMakeLists.txt").string();

    source_setting = "://template/settings-" + config_.compiler + ".json";
    vscode_dir = fs::path(purpose_dir).append(".vscode").string();
    des_setting = fs::path(vscode_dir).append("settings.json").string();

    fs::create_directory(purpose_dir);
    fs::create_directory(vscode_dir);

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
    boost::replace_all(setting, "replaceA", newa);
    boost::replace_all(setting, "replaceB", newb);

    if (setting.empty()) {
        return;
    }
    write_txt(des_setting, setting);
}

void ProjectOpr::handle_main()
{
    QFile::copy(QString::fromStdString(source_main), QString::fromStdString(des_main));
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

std::string ProjectOpr::read_txt(const std::string& path)
{
    std::string   result{};

    QFile file(QString::fromStdString(path));
    if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QString content = file.readAll();
        file.close();
        result.append(content.toLocal8Bit().constData());
    } else {
        std::cout << "读取：" << path << "失败!\n";
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
        std::cout << "打开：" << path << "失败!\n";
        return false;
    }
    out << content;
    out.close();
    return true;
}
