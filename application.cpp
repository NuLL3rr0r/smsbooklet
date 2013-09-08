#include <QtGui/QGuiApplication>
#include "make_unique.hpp"
#include "application.hpp"
#include "mainwindow.hpp"
#include "splashscreen.hpp"

using namespace std;
using namespace SMSDB;

Application::Application(QObject *parent) :
    QObject(parent),
    m_isInitialized(false)
{
}

Application::~Application()
{
}

void Application::Start()
{
    if (m_isInitialized)
        return;

    m_isInitialized = true;

    m_mainWindow = make_unique<MainWindow>();

    QObject::connect(m_mainWindow.get(), SIGNAL(signal_Closed()),
                     this, SLOT(OnMainWindowClosed()));

    m_mainWindow->Show();
    /*m_splashScreen = make_unique<SplashScreen>();

    QObject::connect(m_splashScreen.get(), SIGNAL(signal_Closed()),
                     this, SLOT(OnSplashScreenClosed()));

    //m_splashScreen->Show();
    OnSplashScreenClosed();*/
}

void Application::OnSplashScreenClosed()
{
    /*m_splashScreen->deleteLater();
    m_splashScreen.release();

    m_mainWindow = make_unique<MainWindow>();

    QObject::connect(m_mainWindow.get(), SIGNAL(signal_Closed()),
                     this, SLOT(OnMainWindowClosed()));

    m_mainWindow->Show();*/
}

void Application::OnMainWindowClosed()
{
    m_mainWindow->deleteLater();
    m_mainWindow.release();

    Terminate();
}

void Application::Terminate()
{
    QGuiApplication::exit(0);
}

