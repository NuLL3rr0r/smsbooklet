#ifndef SPLASHSCREEN_HPP
#define SPLASHSCREEN_HPP


#include "window.hpp"

namespace SMSDB {
    class SplashScreen;
}

class SMSDB::SplashScreen : public Window
{
    Q_OBJECT

public:
    explicit SplashScreen(QWindow *parent = 0);
    ~SplashScreen();

private slots:
    void OnTimedOut();
};


#endif // SPLASHSCREEN_HPP
