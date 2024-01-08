#ifndef STRING_HANDLE_HEADER
#define STRING_HANDLE_HEADER

#include <QString>
#include <string>
#include <QMessageBox>
#include <QWidget>

std::string ss(const QString& str);

QString qs(const std::string& str);

void message(QWidget* parent, const QString& info);

#endif