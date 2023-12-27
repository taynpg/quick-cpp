#include <iostream>
#include "src/config.h"
#include "src/project_opr.h"
#include <CLI11.hpp>
#include <string>

#define QUICK_CPP_VERSION "v0.1.0"

bool parse_args(int argc, char** argv, SConfig& config)
{
    std::string intro("快速创建便于 vscode 使用的 cmake 工程\n");
    intro.append(QUICK_CPP_VERSION);
    CLI::App app(intro);

    app.add_option("-t,--type", config.type,
                   "工程类型\n"
                   "[common,一般控制台]\n"
                   "[boost,默认使用boost的工程]\n"
                   "[qt,默认使用qt的工程]");

    app.add_option("-d,--dir", config.project_dir, "工程所在目录");
    app.add_option("-n,--name", config.project_name, "工程名称");

    CLI11_PARSE(app, argc, argv);
    return true;
}

int main(int argc, char** argv)
{
    SConfig config{};
    ConfigOpr configOpr{};
    configOpr.read_config(config);
    
    if (!parse_args(argc, argv, config)) {
        return -1;
    }

    ProjectOpr opr{};
    if (!opr.Run(config)) {
        std::cout << "失败！" << std::endl;
    }
    else {
        std::cout << "成功！" << std::endl;
    }

    return 0;
}
