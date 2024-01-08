#include "MainWidget.h"

#include <QFileDialog>
#include <QMessageBox>

#include "./ui_MainWidget.h"

MainWidget::MainWidget(QWidget* parent)
    : QWidget(parent), ui(new Ui::MainWidget)
{
    ui->setupUi(this);
    setFixedHeight(210);
    setMinimumWidth(910);
    setWindowIcon(QIcon("://resource/ico.ico"));
    setWindowTitle(u8"快速创建code可用的cmake工程 v1.0.2");

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
    if (config.is_export_clangd_ini) {
        ui->cbExportClangdConfig->setCurrentText(u8"是");
    }
    else {
        ui->cbExportClangdConfig->setCurrentText(u8"否");
    }
}

void MainWidget::init_ui()
{
    ui->rbDynamic->setChecked(true);

    ui->cbCompileType->addItem("msvc");
    ui->cbCompileType->addItem("gnu");
    ui->cbCompileType->addItem("clang");
    ui->cbCompileType->setCurrentIndex(0);

    ui->cbProjectType->addItem("console");
    ui->cbProjectType->addItem("qt");
    ui->cbProjectType->addItem("boost");
    ui->cbProjectType->setCurrentIndex(0);

    ui->cbExportClangdConfig->addItem(u8"是");
    ui->cbExportClangdConfig->addItem(u8"否");
    ui->cbExportClangdConfig->setCurrentText(u8"是");

    for (int i = 5; i < 30; ++i) {
        ui->cbFontSize->addItem(QString::number(i));
    }
    ui->cbFontSize->setCurrentText("14");
}

void MainWidget::save_ini(bool is_notice)
{
    SConfig config = read_ui();
    opr_.save_config(config);

    if (is_notice) {
        QMessageBox::information(this, u8"提示", u8"已保存");
    }
}

void MainWidget::generate_project()
{
    save_ini(false);
    SConfig config = read_ui();

    if (config.project_type == "console") {
        project_ = std::make_shared<ProjectConsoleOpr>();
    }
    if (config.project_type == "qt") {
        project_ = std::make_shared<ProjectQtOpr>();
    }
    if (config.project_type == "boost") {
        project_ = std::make_shared<ProjectBoostOpr>();
    }

    if (config.project_name.empty()) {
        QMessageBox::information(this, u8"提示", u8"工程名为空");
        return;
    }

    if ((config.compiler == "gnu" || config.compiler == "clang") && config.query_driver.empty()) {
        QMessageBox::information(this, u8"提示", u8"未填写编译器路径");
        return;
    }

    if (project_->Run(config)) {
        QMessageBox::information(this, u8"提示", u8"已生成");
    } else {
        QMessageBox::information(
            this, u8"提示",
            u8"生成失败，" +
                QString::fromStdString(project_->get_last_error()));
    }
}

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
        SelectFile(this, ui->edCompilePath, u8"选择编译器",
                   u8"可执行文件 (*.exe);;所有文件 (*.*)");
    });

    connect(ui->btnSave, &QPushButton::clicked, this, [=]() { save_ini(); });
    connect(ui->btnGenereate, &QPushButton::clicked, this,
            &MainWidget::generate_project);

    connect(ui->btnExit, &QPushButton::clicked, this,
            [=]() { QApplication::exit(); });
}

SConfig MainWidget::read_ui()
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
    config.project_name = ss(ui->edProjectName->text().trimmed());

    if (ui->cbExportClangdConfig->currentText() == u8"是") {
        config.is_export_clangd_ini = true;
    }
    else {
        config.is_export_clangd_ini = false;
    }

    if (ui->rbStatic->isChecked()) {
        config.is_static = true;
    } else {
        config.is_static = false;
    }
    return config;
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