#ifndef ANDROID_HPP
#define ANDROID_HPP

class QString;
#include <jni.h>

namespace SMSDB {
    class Android;
}

class SMSDB::Android
{
private:
    jobject m_androidObject;

public:
    explicit Android();
    ~Android();

public:
    bool CopyToClipboard(const QString &text);
    bool SendText(const QString &text);
    bool Notify(const QString &text, int duration = 8000);
};


#endif // ANDROID_HPP

