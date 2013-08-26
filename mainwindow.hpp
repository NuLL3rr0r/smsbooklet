#ifndef MAINWINDOW_HPP
#define MAINWINDOW_HPP


#include "window.hpp"

namespace SMSDB {
    class MainWindow;
}

class SMSDB::MainWindow : public Window
{
    Q_OBJECT

public:
    explicit MainWindow(QWindow *parent = 0);
    ~MainWindow();
};


#endif // MAINWINDOW_HPP
