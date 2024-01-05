#include "MainWidget.h"
#include "./ui_MainWidget.h"

MainWidget::MainWidget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::MainWidget)
{
    ui->setupUi(this);
    setFixedHeight(210);
    setMinimumWidth(650);
}

MainWidget::~MainWidget()
{
    delete ui;
}

