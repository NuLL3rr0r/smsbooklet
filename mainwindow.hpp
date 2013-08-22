#ifndef MAINWINDOW_HPP
#define MAINWINDOW_HPP


#include "qtquick2applicationviewer.h"

namespace SMSDB {
    class MainWindow;
}

class SMSDB::MainWindow : public QtQuick2ApplicationViewer
{
    Q_OBJECT

public:
    explicit MainWindow(QWindow *parent = 0);
    ~MainWindow();

signals:
    void signal_Closed();

private slots:
    void OnClosing();
};


#endif // MAINWINDOW_HPP
