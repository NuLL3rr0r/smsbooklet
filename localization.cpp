#include <QDebug>
#include "localization.hpp"

using namespace SMSDB;

void Localization::FormatNumsToPersian(const QString &str, QString &out_str)
{
    out_str.clear();
    for (const auto &c : str) {
        if (c == '0') {
            out_str += "\u06F0";
        } else if (c == '1') {
            out_str += "\u06F1";
        } else if (c == '2') {
            out_str += "\u06F2";
        } else if (c == '3') {
            out_str += "\u06F3";
        } else if (c == '4') {
            out_str += "\u06F4";
        } else if (c == '5') {
            out_str += "\u06F5";
        } else if (c == '6') {
            out_str += "\u06F6";
        } else if (c == '7') {
            out_str += "\u06F7";
        } else if (c == '8') {
            out_str += "\u06F8";
        } else if (c == '9') {
            out_str += "\u06F9";
        } else {
            out_str += c;
        }
    }
}

QString Localization::FormatNumsToPersian(const QString &str)
{
    QString result;
    FormatNumsToPersian(str, result);
    return result;
}

