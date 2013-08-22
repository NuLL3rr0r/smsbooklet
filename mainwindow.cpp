#include "mainwindow.hpp"

using namespace SMSDB;

MainWindow::MainWindow(QWindow *parent) :
    QtQuick2ApplicationViewer(parent)
{
}

MainWindow::~MainWindow()
{
}

void MainWindow::OnClosing()
{
    this->close();
    emit signal_Closed();
}

