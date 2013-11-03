#ifndef LOCALIZATION_HPP
#define LOCALIZATION_HPP


#include <QString>

namespace SMSDB {
    class Localization;
}

class SMSDB::Localization
{
public:
    static void FormatNumsToPersian(const QString &str, QString &out_str);
    static QString FormatNumsToPersian(const QString &str);
};


#endif // LOCALIZATION_HPP
