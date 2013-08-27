#include <QtGui/QGuiApplication>
#include <QtGui/QScreen>
#include <QtQml/QQmlContext>
#include <QtQuick/QQuickItem>
#include "mainwindow.hpp"

using namespace SMSDB;

MainWindow::MainWindow(QWindow *parent) :
    Window(parent)
{
    this->setTitle("پیامک بانک");
    this->setFlags(Qt::Window | Qt::FramelessWindowHint);

    this->SetQML(QStringLiteral("resources/mainwindow.qml"));
}

MainWindow::~MainWindow()
{
}

