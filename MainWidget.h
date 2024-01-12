#ifndef MAINWIDGET_H
#define MAINWIDGET_H

#include <QLineEdit>
#include <QString>
#include <QWidget>
#include <memory>
#include <string>

#include "pub_handle.h"
#include "shortkey.h"
#include "src/config.h"
#include "src/project_opr.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWidget;
}
QT_END_NAMESPACE

class MainWidget : public QWidget {
    Q_OBJECT

public:
    MainWidget(QWidget* parent = nullptr);
    ~MainWidget();

private:
    void    init_ui();
    void    read_ini();
    void    oper();
    SConfig read_ui();

private:
    void save_ini(bool is_notice = true);
    void generate_project();
    void copy_to_clipboard();

private:
    static QString SelectDirectory(QWidget* parent, QLineEdit* pEdit,
                                   const QString& pre_path = "");
    static QString SelectFile(QWidget* parent, QLineEdit* pEdit,
                              const QString& info, const QString& filter);
    static bool    isOk(QWidget* parent, const QString& title,
                        const QString& content);

private:
    Ui::MainWidget*             ui;
    ConfigOpr                   opr_{};
    std::shared_ptr<ProjectOpr> project_{};
};
#endif // MAINWIDGET_H
