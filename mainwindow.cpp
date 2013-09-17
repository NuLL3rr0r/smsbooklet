#include <QtGui/QGuiApplication>
#include <QtGui/QScreen>
#include <QtQml/QQmlContext>
#include <QtQuick/QQuickItem>
#include "make_unique.hpp"
#include "mainwindow.hpp"
#include "pagemodel.hpp"

using namespace std;
using namespace SMSDB;

MainWindow::MainWindow(QWindow *parent) :
    Window(parent)
{
    this->setTitle("پیامک بانک");
    this->setFlags(Qt::Window | Qt::FramelessWindowHint);

    m_pageModel = make_unique<PageModel>();
    m_pages.push_back(make_unique<Page>("import QtQuick 2.1;"
                                        "BorderImage {"
                                        "id: splashImage;"
                                        "source: \"../splashscreen.png\";"
                                        "asynchronous: true;"
                                        "cache: true;"
                                        "smooth: true;"
                                        "anchors.centerIn: parent;"
                                        "anchors.fill: parent;"
                                        "horizontalTileMode: BorderImage.Stretch;"
                                        "verticalTileMode: BorderImage.Stretch;"
                                        "border {"
                                        "top: 24;"
                                        "right: 24;"
                                        "bottom: 24;"
                                        "left: 24;"
                                        "}"
                                        "}"));

    FillCategoryPages();

    for (const auto &p : m_pages) {
        m_pageModel->AddPage(p.get());
    }

    QQmlContext *context = this->rootContext();
    context->setContextProperty("PageModel", m_pageModel.get());

    this->SetQML(QStringLiteral("resources/ui/mainwindow.qml"));
}

MainWindow::~MainWindow()
{
}

void MainWindow::FillCategoryPages()
{

}

