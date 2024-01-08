#include "pub_handle.h"

std::string ss(const QString& str)
{
    return std::string(str.toLocal8Bit().constData());
}

QString qs(const std::string& str)
{
    return QString::fromLocal8Bit(str.c_str());
}

void message(QWidget* parent, const QString& info)
{
    QMessageBox::information(parent, u8"提示", info);
}