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
    bool SendText(const QString &text);
};


#endif // ANDROID_HPP

