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
    m_fontPath("assets:/resources/fnt/BYekan.ttf"),
    m_imagePath("assets:/resources/img/"),
#else
    m_fontPath("file:" + QDir::currentPath() + "/resources/fnt/BYekan.ttf"),
    m_imagePath("file:" + QDir::currentPath() + "/resources/img/"),
#endif
  m_pageBgImages {
{Window::DisplayRatio::Vert_10_16, m_imagePath + "about_bg_1200x1920.png"},
{Window::DisplayRatio::Vert_9_16, m_imagePath + "about_bg_1080x1920.png"},
{Window::DisplayRatio::Vert_3_4, m_imagePath + "about_bg_1440x1920.png"}
      }
{
    this->setTitle(APP_TITLE);
    this->setFlags(Qt::Window | Qt::FramelessWindowHint);

    QQmlContext *context = this->rootContext();
    context->setContextProperty("FontPath", m_fontPath);
    context->setContextProperty("BackgroundPath", m_pageBgImages[GetDisplayRatio()]);
    context->setContextProperty("AppLogoPath", m_imagePath + "smsbooklet-480x380.png");
    context->setContextProperty("BrandingLogoPath", m_imagePath + "approsia_144x50.png");

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

