#ifndef SHORTKEY_H
#define SHORTKEY_H

#include "pub_handle.h"
#include <QDialog>
#include <QString>
#include <json/json.hpp>

namespace Ui {
class ShortKey;
}

class ShortKey : public QDialog {
    Q_OBJECT

public:
    explicit ShortKey(QWidget* parent = nullptr);
    ~ShortKey();

private:
    void    key_debug_set();
    void    key_define();
    QString get_keybindings_file();

private:
    void read_json(const QString& file, bool notice = true);
    void update_or_insert(const nlohmann::json& node,
                          const std::string&    search_value);
    bool search_node(const std::string& search_value, nlohmann::json& node);
    void delete_node(const std::string& search_value);
    bool save_json();

private:
    Ui::ShortKey*  ui;
    nlohmann::json json_array_;
    QString        file_{};
};

#endif // SHORTKEY_H
