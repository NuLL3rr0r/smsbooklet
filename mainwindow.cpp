#include <cassert>
#include <cmath>
#include <QtCore/QString>
#include <QtGui/QGuiApplication>
#include <QtGui/QScreen>
#include <QtQml/QQmlContext>
#include <QtQuick/QQuickItem>
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlError>
#include <QtSql/QSqlQuery>
#include <QtSql/QSqlRecord>
#include "make_unique.hpp"
#include "mainwindow.hpp"
#include "messagebrowser.hpp"
#include "pagemodel.hpp"
#include "rt.hpp"

#define     UI_FILE            "qrc:/ui/mainwindow.qml"

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

    this->SetQML(QStringLiteral(UI_FILE));
}

MainWindow::~MainWindow()
{
}

void MainWindow::OnMessageBrowserClosed() {
    this->setVisible(true);
    m_messageBrowser.reset();
}

void MainWindow::OnMessageBrowserShown() {
    this->setVisible(false);
}

void MainWindow::loadMessages(QString category) {
    m_messageBrowser = make_unique<MessageBrowser>(category);
    QObject::connect(m_messageBrowser.get(), SIGNAL(signal_Closed()),
                     this, SLOT(OnMessageBrowserClosed()));
    QObject::connect(m_messageBrowser.get(), SIGNAL(signal_Shown()),
                     this, SLOT(OnMessageBrowserShown()));
    m_messageBrowser->Show();
}

void MainWindow::FillCategoryPages()
{
    QSqlQuery query(" SELECT TRIM(category) AS cat_col "
                    " FROM messages "
                    " GROUP BY cat_col "
                    " ORDER BY cat_col; ");
    QSqlRecord record = query.record();

    const double padding = 96.0;
    const double spacing = 48.0;
    const double pageNumberMargin = 36.0;
    const unsigned char maxRow = 4;
    const unsigned char maxCol = 2;
    unsigned char r = 0;
    unsigned char c = 0;
    QString page;

    const double buttonWidth = (getScreenWidth() - ((padding * 2.0) + ((maxCol - 1.0) * spacing))) / maxCol;
    const double buttonHeight = (getScreenHeight() - ((padding * 2.0) + ((maxRow - 1.0) * spacing))) / maxRow;
    const double pageConentsHeight = getScreenHeight() - (padding * 2.0);

    size_t queryCount = -1;
    while(query.next()) {
        ++queryCount;
    }
    query.first();
    queryCount = ceil((double)queryCount / (double)(maxRow * maxCol));

    size_t i = 0;
    while (query.next()) {
        if (r == 0 && c == 0) {
            page.clear();
            page = QString("import QtQuick 2.1;"
                           "import QtQuick.Controls 1.0;"
                           "import QtQuick.Controls.Styles 1.0;"
                           "import QtQuick.Layouts 1.0;"
                           "Rectangle {"
                           "anchors.centerIn: parent;"
                           "anchors.fill: parent;"
                           "property Component buttonStyle: ButtonStyle {"
                           "id: buttonStyle;"
                           "background: Rectangle {"
                           "color: control.pressed ? 'darkGray' : 'lightGray';"
                           "antialiasing: true;"
                           "border.color: 'gray';"
                           "radius: 0.0;"
                           "}"
                           "}"
                           "Rectangle {"
                           "anchors.fill: parent;"
                           "anchors.centerIn: parent;"
                           "Column {"
                           "anchors.centerIn: parent;"
                           "spacing: %1;"
                           "height: %2;").arg(spacing).arg(pageConentsHeight);
        }

        if (c == 0) {
            page += QString("Row {"
                            "spacing: %1;").arg(spacing);
        }

        QString category = query.value(record.indexOf("cat_col")).toString();
        page += QString("Column {"
                        "Button {"
                        "style: buttonStyle;"
                        "Text {"
                        "anchors.fill: parent;"
                        "anchors.centerIn: parent;"
                        "verticalAlignment: Text.AlignVCenter;"
                        "horizontalAlignment: Text.AlignHCenter;"
                        "wrapMode: Text.WordWrap;"
                        "text: \"%1\";"
                        "}"
                        "width: %2;"
                        "height: %3;"
                        "onClicked: {"
                        "loadMessages('%1');"
                        "}"
                        "}"
                        "}"
                        ).arg(category).arg(buttonWidth).arg(buttonHeight);

        if (c == maxCol - 1) {
            page += "}";    // close the row

            if (r == maxRow - 1) {
                page += QString("}"     // close the rectangle
                                "}"     // close the column
                                "Text {"
                                "anchors {"
                                "bottom: parent.bottom;"
                                "horizontalCenter: parent.horizontalCenter;"
                                "bottomMargin: %1;"
                                "}"
                                "wrapMode: Text.NoWrap;"
                                "text: \"%2 / %3\";"
                                "}"
                                "}").arg(pageNumberMargin)
                        .arg(++i).arg(queryCount);    // close the rectangle
                m_pages.push_back(make_unique<Page>(page));
            }

            r = (r != maxRow - 1) ? r + 1 : 0;
        }

        c = (c != maxCol - 1) ? c + 1 : 0;
    }

    if (c != 0) {
        page += "}";    // close the row
    }

    if (r != 0) {
        page += QString("}"     // close the rectangle
                        "}"     // close the column
                        "Text {"
                        "anchors {"
                        "bottom: parent.bottom;"
                        "horizontalCenter: parent.horizontalCenter;"
                        "bottomMargin: %1;"
                        "}"
                        "wrapMode: Text.NoWrap;"
                        "text: \"%2 / %3\";"
                        "}"
                        "}").arg(pageNumberMargin)
                .arg(++i).arg(queryCount);    // close the rectangle
        m_pages.push_back(make_unique<Page>(page));
    }
}

