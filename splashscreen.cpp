#include <QQmlContext>
#include <QTimer>
#include "splashscreen.hpp"

#define     SPLASH_TIMEOUT          1500

using namespace SMSDB;

SplashScreen::SplashScreen(QWindow *parent) :
    QtQuick2ApplicationViewer(parent)
{
    this->setTitle("");
    this->setFlags(Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint);

    this->rootContext()->setContextProperty("cppSplashScreen", this);
    this->setMainQmlFile(QStringLiteral("resources/splashscreen.qml"));

    QTimer::singleShot(SPLASH_TIMEOUT, this, SLOT(OnTimedOut()));
}

SplashScreen::~SplashScreen()
{
}

void SplashScreen::OnTimedOut()
{
    emit signal_TimedOut();
}

