#ifndef APPLICATION_HPP
#define APPLICATION_HPP


#include <memory>
#include <QObject>

namespace SMSDB {
    class Application;
    class MainWindow;
    class SplashScreen;
}

class SMSDB::Application : public QObject
{
    Q_OBJECT

private:
    bool m_isInitialized;

    std::unique_ptr<SMSDB::MainWindow> m_mainWindow;
    std::unique_ptr<SMSDB::SplashScreen> m_splashScreen;

public:
    explicit Application(QObject *parent = 0);
    ~Application();
    
signals:
    
public slots:

private slots:
    void OnSplashScreenTimedOut();
    void OnMainWindowClosed();

public:
    void Start();

private:
    void Terminate();
};


#endif // APPLICATION_HPP
