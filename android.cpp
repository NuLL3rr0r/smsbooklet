#include <QDebug>
#include "android.hpp"

using namespace SMSDB;

static JavaVM *s_javaVM = NULL;
static jclass s_androidClassID = NULL;
static jmethodID s_androidConstructorMethodID = NULL;
static jmethodID s_androidSendTextMethodID = NULL;
static jmethodID s_androidReleaseMethodID = NULL;

// This method is called immediately after the module is loaded.
JNIEXPORT jint JNI_OnLoad(JavaVM *vm, void *)
{
    JNIEnv *env;
    if (vm->GetEnv(reinterpret_cast<void **>(&env), JNI_VERSION_1_6) != JNI_OK) {
        qCritical() << "  * Could not get the enviroument !!";
        return -1;
    }

    s_javaVM = vm;

    // search for our class
    jclass clazz = env->FindClass("smsdb/Android");
    if (!clazz) {
        qCritical() << "  * Could not find the Android class !!";
        return -1;
    }

    // Keep a global reference to it.
    s_androidClassID = (jclass)env->NewGlobalRef(clazz);

    // Search for its contructor.
    s_androidConstructorMethodID = env->GetMethodID(
                s_androidClassID, "<init>", "()V");
    if (!s_androidConstructorMethodID) {
        qCritical() << "  * Could not find the Android class contructor !!";
        return -1;
    }

    // search for Release method
    s_androidReleaseMethodID = env->GetMethodID(
                s_androidClassID, "Release", "()Z");
    if (!s_androidReleaseMethodID) {
        qCritical() << "  * Could not find Release method !!";
        return -1;
    }

    // Search for SendText method.
    s_androidSendTextMethodID = env->GetMethodID(
                s_androidClassID, "SendText", "(Ljava/lang/String;)Z");
    if (!s_androidSendTextMethodID) {
        qCritical() << "  * Could not find SendText method !!";
        return -1;
    }

    qDebug() << "  - JNI_OnLoad was executed successfully!";

    return JNI_VERSION_1_6;
}

Android::Android()
{
    JNIEnv *env;

    // Qt is running in a different thread than Java UI.
    // So, Java VM *MUST* always stay attached to the current thread.
    if (s_javaVM->AttachCurrentThread(&env, NULL) < 0) {
        qCritical() << "  * AttachCurrentThread failed !!";
        return;
    }

    // Create a new instance of Android
    m_androidObject = env->NewGlobalRef(
                env->NewObject(s_androidClassID,
                               s_androidConstructorMethodID));
    if (!m_androidObject) {
        qCritical() << "  * Could not create the Android class instance !!";
        return;
    }

    // Don't forget to detach from current thread.
    s_javaVM->DetachCurrentThread();
}

Android::~Android()
{
    if (m_androidObject) {
        JNIEnv *env;

        if (s_javaVM->AttachCurrentThread(&env, NULL) < 0 ) {
            qCritical() << "  * AttachCurrentThread failed !!";
            return;
        }

        if (!env->CallBooleanMethod(m_androidObject,
                                    s_androidReleaseMethodID)) {
            qCritical() << "  * Releasing Android object failed !!";
        }

        s_javaVM->DetachCurrentThread();
    }
}

bool Android::SendText(const QString &text)
{
    if (!m_androidObject)
        return false;

    JNIEnv *env;
    if (s_javaVM->AttachCurrentThread(&env, NULL) < 0) {
        qCritical() << "AttachCurrentThread failed !!";
        return false;
    }

    jstring str = env->NewString(reinterpret_cast<const jchar *>(
                                     text.constData()), text.length());

    jboolean res = env->CallBooleanMethod(
                m_androidObject, s_androidSendTextMethodID, str);
    env->DeleteLocalRef(str);
    s_javaVM->DetachCurrentThread();

    return res;
}

