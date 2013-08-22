#ifndef SPLASHSCREEN_HPP
#define SPLASHSCREEN_HPP


#include "qtquick2applicationviewer.h"

namespace SMSDB {
    class SplashScreen;
}

class SMSDB::SplashScreen : public QtQuick2ApplicationViewer
{
    Q_OBJECT

public:
    explicit SplashScreen(QWindow *parent = 0);
    ~SplashScreen();
    
signals:
    void signal_TimedOut();

private slots:
    void OnTimedOut();
};


#endif // SPLASHSCREEN_HPP
