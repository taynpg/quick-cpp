#include "shortkey.h"
#include "ui_shortkey.h"
#include <QDir>
#include <QMessageBox>
#include <fstream>

constexpr auto VALUE_COMMAND_DEBUG = "cmake.debugTarget";
constexpr auto VALUE_COMMAND_DEFINE = "editor.action.revealDefinition";
constexpr auto KEY_COMMAND = "command";
constexpr auto KEY_KEY = "key";
constexpr auto KEY_WHEN = "when";

ShortKey::ShortKey(QWidget* parent) : QDialog(parent), ui(new Ui::ShortKey)
{
    ui->setupUi(this);
    setWindowTitle(u8"快捷键设置");
    setFixedSize(350, 70);

    file_ = get_keybindings_file();
    read_json(file_, false);

    nlohmann::json node{};
    if (search_node(VALUE_COMMAND_DEBUG, node)) {
        ui->edDebug->setText(qs(node[KEY_KEY]));
    }
    if (search_node(VALUE_COMMAND_DEFINE, node)) {
        ui->edToDefine->setText(qs(node[KEY_KEY]));
    }

    connect(ui->btnDebugSet, &QPushButton::clicked, this,
            [=]() { key_debug_set(); });
    connect(ui->btnDebugUnSet, &QPushButton::clicked, this, [=]() {
        ui->edDebug->clear();

        delete_node(VALUE_COMMAND_DEBUG);
    });

    connect(ui->btnDefineSet, &QPushButton::clicked, this,
            [=]() { key_define(); });
    connect(ui->btnDefineUnSet, &QPushButton::clicked, this, [=]() {
        ui->edToDefine->clear();
        delete_node(VALUE_COMMAND_DEFINE);
    });
}

ShortKey::~ShortKey() { delete ui; }

void ShortKey::key_debug_set()
{
    json_array_.clear();
    if (ui->edDebug->text().trimmed().isEmpty()) {
        message(this, u8"未填写内容");
        return;
    }
    if (!QFile::exists(file_)) {
        json_array_ = nlohmann::json::array();
    } else {
        read_json(file_);
    }

    nlohmann::json json_node = {
        {KEY_KEY, ss(ui->edDebug->text().trimmed())},
        {KEY_COMMAND, VALUE_COMMAND_DEBUG},
        {KEY_WHEN, "cmake:enableFullFeatureSet && inCMakeProject && "
                   "!cmake:hideDebugCommand && !inDebugMode"}};

    update_or_insert(json_node, VALUE_COMMAND_DEBUG);
    save_json();
    message(this, u8"完成");
}

bool ShortKey::save_json()
{
    std::ofstream out(ss(file_));
    if (out.is_open()) {
        out << std::setw(4) << json_array_ << std::endl;
        out.close();
        return true;
    } else {
        message(this, u8"未能打开文件" + file_);
        return false;
    }
}

void ShortKey::read_json(const QString& file, bool notice)
{
    std::ifstream in(ss(file_));
    if (!in.is_open()) {
        if (notice) {
            message(this, u8"未能打开文件" + file_);
        }
        return;
    }
    in >> json_array_;
}

void ShortKey::update_or_insert(const nlohmann::json& node,
                                const std::string&    search_value)
{
    bool is_find = false;
    for (auto& data : json_array_) {
        if (data.contains(KEY_COMMAND) && data[KEY_COMMAND] == search_value) {
            data[KEY_KEY] = node[KEY_KEY];
            data[KEY_WHEN] = node[KEY_WHEN];
            is_find = true;
            break;
        }
    }
    if (!is_find) {
        json_array_.push_back(node);
    }
}

bool ShortKey::search_node(const std::string& search_value,
                           nlohmann::json&    node)
{
    bool                           is_find = false;
    nlohmann::json::const_iterator it;
    for (it = json_array_.begin(); it != json_array_.end(); ++it) {
        if (it->contains(KEY_COMMAND) && (*it)[KEY_COMMAND] == search_value) {
            node = *it;
            is_find = true;
            break;
        }
    }
    return is_find;
}

void ShortKey::delete_node(const std::string& search_value)
{
    nlohmann::json::iterator it;
    for (it = json_array_.begin(); it != json_array_.end(); ++it) {
        if (it->contains(KEY_COMMAND) && (*it)[KEY_COMMAND] == search_value) {
            it = json_array_.erase(it);
            break;
        }
    }
    save_json();
    message(this, u8"完成");
}

void ShortKey::key_define()
{
    json_array_.clear();
    if (ui->edToDefine->text().trimmed().isEmpty()) {
        message(this, u8"未填写内容");
        return;
    }
    if (!QFile::exists(file_)) {
        json_array_ = nlohmann::json::array();
    } else {
        read_json(file_);
    }

    nlohmann::json json_node = {
        {KEY_KEY, ss(ui->edToDefine->text().trimmed())},
        {KEY_COMMAND, VALUE_COMMAND_DEFINE},
        {KEY_WHEN, "editorHasDefinitionProvider && editorTextFocus && "
                   "!isInEmbeddedEditor"}};

    update_or_insert(json_node, VALUE_COMMAND_DEFINE);
    save_json();
    message(this, u8"完成");
}

QString ShortKey::get_keybindings_file()
{
    QString path{};

#ifdef _WIN32
    QString home_dir = QDir::homePath();
    QDir    dir(home_dir);
    QString tmp = dir.filePath("AppData/Roaming/Code/User");
    path = QDir(tmp).absoluteFilePath("keybindings.json");
#endif

    return path;
}