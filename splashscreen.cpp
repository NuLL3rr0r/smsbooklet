#include <QtCore/QTimer>
#include <QtQml/QQmlContext>
#include <QtQuick/QQuickItem>
#include "splashscreen.hpp"

using namespace SMSDB;

SplashScreen::SplashScreen(QWindow *parent) :
    Window(parent)
{
    this->setTitle("پیامک بانک");
    this->setFlags(Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint);

    this->SetQML(QStringLiteral("resources/splashscreen.qml"));

    QObject *rootObject = this->rootObject();
    QObject::connect(rootObject, SIGNAL(signal_TimedOut()),
                     this, SLOT(OnTimedOut()));
}

SplashScreen::~SplashScreen()
{
}

void SplashScreen::OnTimedOut()
{
    this->Close();
}

