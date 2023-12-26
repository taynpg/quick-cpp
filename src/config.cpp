#include "config.h"

#include <QSettings>


constexpr auto DEFAULT_PROJECT_TYPE = "DefaultProjectType";
constexpr auto QT_DIR = "QtDir";
constexpr auto BOOST_DIR = "BoostDir";
constexpr auto COMPILER = "Compiler";
constexpr auto FONT_SIZE = "FontSize";
constexpr auto FONT_FAMILY = "FontFamily";
constexpr auto QUERY_DRIVER = "QueryDriver";

void ConfigOpr::read_config(SConfig& config)
{
    QString home = QDir::homePath();
    QString config_dir = home + "/.config/quick-cpp";
    QString logfile = config_dir + "/config.ini";
    config.config_path.append(logfile.toLocal8Bit().constData());

    QDir dir(config_dir);
    if (!dir.exists()) {
        dir.mkdir(config_dir);
    }
    if (!QFile::exists(logfile)) {
        set_default(logfile);
        return;
    }
}

void ConfigOpr::set_default(const QString& config_path)
{
    QSettings* settings = new QSettings(config_path, QSettings::IniFormat);
    settings->beginGroup("Basic");

    // common,boost,qt
    settings->setValue(DEFAULT_PROJECT_TYPE, "common");

    settings->setValue(QT_DIR, R"(C:\Qt\Qt5.14.2\5.14.2\msvc2017_64)");
    settings->setValue(BOOST_DIR, R"(C:\Bin\Boost)");
    settings->setValue(FONT_SIZE, 15);
    settings->setValue(FONT_FAMILY, "FiraCode Nerd Font Mono");
    settings->setValue(QUERY_DRIVER, "/usr/bin/g++");

    // msvc,gnu,clang
    settings->setValue(COMPILER, "msvc");

    settings->endGroup();
    delete settings;
}
