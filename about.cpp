#include <QtCore/QDir>
#include <QtGui/QGuiApplication>
#include <QtGui/QScreen>
#include <QtQml/QQmlContext>
#include <QtQuick/QQuickItem>
#include "about.hpp"
#include "defs.hpp"

#define     UI_FILE            "qrc:/ui/about.qml"

using namespace std;
using namespace SMSDB;

#if defined(Q_OS_ANDROID)
About::About(keyPressHandler_ptr keyPressHandler, QWindow *parent) :
#else
About::About(QWindow *parent) :
#endif
    Window(parent),
#if defined(Q_OS_ANDROID)
    m_keyPressHandler(keyPressHandler),
    m_hasBeenClosed(false),
    m_imagePath("assets:/resources/img/")
#else
    m_imagePath("file:" + QDir::currentPath() + "/resources/img/")
#endif
{
    this->setTitle(APP_TITLE);
    this->setFlags(Qt::Window | Qt::FramelessWindowHint);

    this->SetQML(QStringLiteral(UI_FILE));
}

About::~About()
{
}

#if defined(Q_OS_ANDROID)
void About::Close()
{
    emit signal_Closed();
    this->hide();
    m_hasBeenClosed = true;
}

void About::keyPressEvent(QKeyEvent *e)
{
    if (e->key() != Qt::Key_MediaPrevious) {
        if (!m_hasBeenClosed) {
            QtQuick2ApplicationViewer::keyPressEvent(e);
        } else {
            this->m_keyPressHandler(e);
        }
    } else {
        if (!m_hasBeenClosed) {
            Close();
        } else {
            this->m_keyPressHandler(e);
        }
    }
}
#endif

