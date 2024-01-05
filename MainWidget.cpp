#include "MainWidget.h"

#include <QFileDialog>
#include <QMessageBox>

#include "./ui_MainWidget.h"

std::string ss(const QString& str)
{
    return std::string(str.toLocal8Bit().constData());
}

QString qs(const std::string& str)
{
    return QString::fromLocal8Bit(str.c_str());
}

MainWidget::MainWidget(QWidget* parent)
    : QWidget(parent), ui(new Ui::MainWidget)
{
    ui->setupUi(this);
    setFixedHeight(210);
    setMinimumWidth(810);
    setWindowIcon(QIcon("://resource/ico.ico"));
    setWindowTitle(u8"快速创建方便code使用的cmak工程");

    init_ui();
    read_ini();
    oper();
}

MainWidget::~MainWidget() { delete ui; }

void MainWidget::read_ini()
{
    SConfig config{};
    opr_.read_config(config);

    ui->edProjectDir->setText(qs(config.project_dir));
    ui->edQtPath->setText(qs(config.qt_dir));
    ui->edBoostPath->setText((qs(config.boost_dir)));
    ui->edCompilePath->setText(qs(config.query_driver));

    if (!config.compiler.empty()) {
        ui->cbCompileType->setCurrentText(qs(config.compiler));
    }

    if (!config.project_type.empty()) {
        ui->cbProjectType->setCurrentText(qs(config.project_type));
    }

    if (config.font_size > 4) {
        ui->cbFontSize->setCurrentText(QString::number(config.font_size));
    }
    if (!config.font_family.empty()) {
        ui->fontComboBox->setCurrentFont(QFont(qs(config.font_family)));
    }
}

void MainWidget::init_ui()
{
    ui->cbCompileType->addItem("msvc");
    ui->cbCompileType->addItem("gnu");
    ui->cbCompileType->addItem("clang");
    ui->cbCompileType->setCurrentIndex(0);

    ui->cbProjectType->addItem("console");
    ui->cbProjectType->addItem("qt");
    ui->cbProjectType->addItem("boost");
    ui->cbProjectType->setCurrentIndex(0);

    for (int i = 5; i < 30; ++i) {
        ui->cbFontSize->addItem(QString::number(i));
    }
    ui->cbFontSize->setCurrentText("14");
}

void MainWidget::save_ini(bool is_notice)
{
    SConfig config{};
    config.project_type = ss(ui->cbProjectType->currentText());
    config.project_dir = ss(ui->edProjectDir->text());
    config.font_size = ui->cbFontSize->currentText().toInt();
    config.font_family = ss(ui->fontComboBox->currentFont().family());
    config.boost_dir = ss(ui->edBoostPath->text());
    config.qt_dir = ss(ui->edQtPath->text());
    config.compiler = ss(ui->cbCompileType->currentText());
    config.query_driver = ss(ui->edCompilePath->text());
    opr_.save_config(config);

    if (is_notice) {
        QMessageBox::information(this, u8"提示", "已保存");
    }
}

void MainWidget::generate_project() { save_ini(); }

void MainWidget::oper()
{
    QString pre_boost_path{};
    QString pre_qt_path{};
    QString pre_project_path{};
    connect(ui->btnSelectBoost, &QPushButton::clicked, this,
            [=]() { SelectDirectory(this, ui->edBoostPath, pre_boost_path); });
    connect(ui->btnSelectProjectDir, &QPushButton::clicked, this, [=]() {
        SelectDirectory(this, ui->edProjectDir, pre_project_path);
    });
    connect(ui->btnSelectQt, &QPushButton::clicked, this,
            [=]() { SelectDirectory(this, ui->edQtPath, pre_qt_path); });
    connect(ui->btnCompile, &QPushButton::clicked, this, [=]() {
        SelectFile(this, ui->edQtPath, u8"选择编译器",
                   u8"可执行文件 (*.exe);;所有文件 (*.*)");
    });

    connect(ui->btnSave, &QPushButton::clicked, this, [=]() { save_ini(); });
    connect(ui->btnGenereate, &QPushButton::clicked, this,
            &MainWidget::generate_project);
}

QString MainWidget::SelectDirectory(QWidget* parent, QLineEdit* pEdit,
                                    const QString& pre_path)
{
    QString start_path = pre_path;
    QDir    folder(start_path);
    if (start_path.isEmpty() || !folder.exists()) {
        start_path = QDir::homePath();
    }
    QString selectedDirectory =
        QFileDialog::getExistingDirectory(parent, u8"选择文件夹", start_path);

    if (selectedDirectory.isEmpty()) {
        return "";
    }
    pEdit->setText(selectedDirectory);
    return selectedDirectory;
}

QString MainWidget::SelectFile(QWidget* parent, QLineEdit* pEdit,
                               const QString& info, const QString& filter)
{
    QString filePath =
        QFileDialog::getOpenFileName(parent, info, QDir::homePath(), filter);

    if (!filePath.isEmpty()) {
        pEdit->setText(filePath);
    }
    return filePath;
}

bool MainWidget::isOk(QWidget* parent, const QString& title,
                      const QString& content)
{
    QMessageBox questionBox(parent);
    questionBox.setText(content);
    questionBox.setWindowTitle(title);
    questionBox.setIcon(QMessageBox::Question);
    questionBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
    int result = questionBox.exec();

    if (result != QMessageBox::Yes) {
        return false;
    } else {
        return true;
    }
}