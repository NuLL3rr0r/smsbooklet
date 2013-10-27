#include <cassert>
#include <cmath>
#include <QtCore/QDir>
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
#include "about.hpp"
#ifdef Q_OS_ANDROID
#include "android.hpp"
#endif
#include "defs.hpp"
#include "messagebrowser.hpp"
#include "pagemodel.hpp"
#include "rt.hpp"
#include "subcategorybrowser.hpp"

#define     UI_FILE            "qrc:/ui/mainwindow.qml"

using namespace std;
using namespace SMSDB;

MainWindow::MainWindow(QWindow *parent) :
    Window(parent),
#ifdef Q_OS_ANDROID
    m_imagePath("assets:/resources/img/"),
#else
    m_imagePath("file:" + QDir::currentPath() + "/resources/img/"),
#endif
    m_splashScreenImages {
{Window::DisplayRatio::Horz_16_10, m_imagePath + "splashscreen_1920x1200.jpg"},
{Window::DisplayRatio::Horz_16_9, m_imagePath + "splashscreen_1920x1080.jpg"},
{Window::DisplayRatio::Horz_4_3, m_imagePath + "splashscreen_1920x1440.jpg"},
{Window::DisplayRatio::Vert_10_16, m_imagePath + "splashscreen_1200x1920.jpg"},
{Window::DisplayRatio::Vert_9_16, m_imagePath + "splashscreen_1080x1920.jpg"},
{Window::DisplayRatio::Vert_3_4, m_imagePath + "splashscreen_1440x1920.jpg"}
        }
{
    this->setTitle(APP_TITLE);
    this->setFlags(Qt::Window | Qt::FramelessWindowHint);

    m_pageModel = make_unique<PageModel>();
    m_pages.push_back(make_unique<Page>(
                          QString("import QtQuick 2.1;"
                                  "BorderImage {"
                                  "id: splashImage;"
                                  "source: \"%1\";"
                                  "asynchronous: true;"
                                  "cache: true;"
                                  "smooth: true;"
                                  "anchors.centerIn: parent;"
                                  "anchors.fill: parent;"
                                  "horizontalTileMode: BorderImage.Stretch;"
                                  "verticalTileMode: BorderImage.Stretch;"
                                  "border {"
                                  "top: 0;"
                                  "right: 0;"
                                  "bottom: 0;"
                                  "left: 0;"
                                  "}"
                                  "}")
                          .arg(m_splashScreenImages[GetDisplayRatio()])));

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

void MainWindow::OnAboutClosed()
{
    this->setVisible(true);
#if !defined(Q_OS_ANDROID)
    m_about.reset();
#endif
}

void MainWindow::OnAboutShown()
{
    this->setVisible(false);
}

void MainWindow::OnSubCategoryBrowserClosed()
{
    this->setVisible(true);
#if !defined(Q_OS_ANDROID)
    m_subCategoryBrowser.reset();
#endif
}

void MainWindow::OnSubCategoryBrowserShown()
{
    this->setVisible(false);
}

void MainWindow::OnMessageBrowserClosed()
{
    this->setVisible(true);
#if !defined(Q_OS_ANDROID)
    m_messageBrowser.reset();
#endif
}

void MainWindow::OnMessageBrowserShown()
{
    this->setVisible(false);
}

void MainWindow::about()
{
#if defined(Q_OS_ANDROID)
    m_about.reset();
    m_about = make_unique<About>(std::bind(
                                     &MainWindow::keyPressEvent,
                                     this,
                                     std::placeholders::_1));
#else
    m_about = make_unique<About>();
#endif
    QObject::connect(m_about.get(), SIGNAL(signal_Closed()),
                     this, SLOT(OnAboutClosed()));
    QObject::connect(m_about.get(), SIGNAL(signal_Shown()),
                     this, SLOT(OnAboutShown()));
    m_about->Show();
}

void MainWindow::browseSubCategories(QString category)
{
#if defined(Q_OS_ANDROID)
    m_subCategoryBrowser.reset();
    m_subCategoryBrowser = make_unique<SubCategoryBrowser>(category,
                                                           std::bind(
                                                               &MainWindow::keyPressEvent,
                                                               this,
                                                               std::placeholders::_1));
#else
    m_subCategoryBrowser = make_unique<SubCategoryBrowser>(category);
#endif
    QObject::connect(m_subCategoryBrowser.get(), SIGNAL(signal_Closed()),
                     this, SLOT(OnSubCategoryBrowserClosed()));
    QObject::connect(m_subCategoryBrowser.get(), SIGNAL(signal_Shown()),
                     this, SLOT(OnSubCategoryBrowserShown()));
    m_subCategoryBrowser->Show();
}

void MainWindow::browseMessages(QString category)
{
    QSqlQuery query(" SELECT rowid "
                    " FROM messages "
                    " WHERE fav = 1; ");
    if (!query.next()) {
        QVariant returnedValue;
        QVariant msg = "هنوز چیزی به لیست علاقه مندی ها اضافه نشده است.";
        QMetaObject::invokeMethod(this->rootObject(), "notify",
                                  Q_RETURN_ARG(QVariant, returnedValue),
                                  Q_ARG(QVariant, msg));
        return;
    }

#if defined(Q_OS_ANDROID)
    m_messageBrowser.reset();
    m_messageBrowser = make_unique<MessageBrowser>(category,
                                                   std::bind(&MainWindow::keyPressEvent,
                                                             this,
                                                             std::placeholders::_1));
#else
    m_messageBrowser = make_unique<MessageBrowser>(category);
#endif
    QObject::connect(m_messageBrowser.get(), SIGNAL(signal_Closed()),
                     this, SLOT(OnMessageBrowserClosed()));
    QObject::connect(m_messageBrowser.get(), SIGNAL(signal_Shown()),
                     this, SLOT(OnMessageBrowserShown()));
    m_messageBrowser->Show();
}

void MainWindow::FillCategoryPages()
{
    QSqlQuery query(" SELECT TRIM ( name ) AS name_col, icon "
                    " FROM categories "
                    " GROUP BY name_col "
                    " ORDER BY name_col; ");
    QSqlRecord record = query.record();

    const double buttonWidth = 144.0;
    const double buttonHeight = 144.0;
    const double spacing = 8.0;
    const double minPadding = 80.0;
    const unsigned char maxCol = std::floor(
                ((getScreenWidth() - (minPadding * 2.0)) + spacing) / (buttonWidth + spacing));
    const unsigned char maxRow = std::floor(
                ((getScreenHeight() - (minPadding * 2.0)) + spacing) / (buttonHeight + spacing));
    const double paddingW = (getScreenWidth() -
            ((buttonWidth * maxCol) + (spacing * (maxCol - 1)))) / 2.0;
    const double paddingH = (getScreenHeight() -
            ((buttonHeight * maxRow) + (spacing * (maxRow - 1)))) / 2.0;
    const double pageContentsHeight = getScreenHeight() - (paddingH * 2.0);
    const double pageContentsWidth = getScreenWidth() - (paddingW * 2.0);
    const double pageNumberMargin = paddingH / 2.0;

    unsigned char r = 0;
    unsigned char c = 0;
    QString page;

    size_t queryCount = 0;
    bool insertedAbout = false;
    bool insertedFav = false;

    while(query.next()) {
        ++queryCount;
    }
    query.first();
    query.previous();
    queryCount = ceil((double)queryCount / (double)(maxRow * maxCol));

    bool isRowClosed = false;
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
                           "Rectangle {"
                           "anchors.fill: parent;"
                           "anchors.centerIn: parent;"
                           "Column {"
                           "anchors.centerIn: parent;"
                           "spacing: %1;"
                           "width: %2;"
                           "height: %3;").arg(spacing)
                    .arg(pageContentsWidth).arg(pageContentsHeight);
        }

        if (c == 0) {
            page += QString("Row {"
                            "anchors.left: parent.left;"
                            "spacing: %1;").arg(spacing);
        }

        QString category;
        QString icon;

        if (insertedAbout) {
            if (insertedFav) {
                category = query.value(record.indexOf("name_col")).toString();
                icon = query.value(record.indexOf("icon")).toString();
            } else {
                insertedFav = true;
                category = FAV_BUTTON_TEXT;
                icon = "favourite_cat_btn_144x144.png";
            }

            page += QString("Column {"
                            "Image {"
                            "source: '%4%5';"
                            "width: %2;"
                            "height: %3;"
                            "MouseArea {"
                            "anchors.fill: parent;"
                            "onClicked: {"
                            "cppWindow.%6('%1');"
                            "}"
                            "}"
                            "}"
                            "}"
                            ).arg(category).arg(buttonWidth).arg(buttonHeight)
                    .arg(m_imagePath).arg(icon)
                    .arg(category != FAV_BUTTON_TEXT ? "browseSubCategories"
                                                     : "browseMessages");
        } else {
            insertedAbout = true;
            icon = "about_btn_144x144.png";

            page += QString("Column {"
                            "Image {"
                            "source: '%3about_btn_144x144.png';"
                            "width: %1;"
                            "height: %2;"
                            "MouseArea {"
                            "anchors.fill: parent;"
                            "onClicked: {"
                            "cppWindow.about();"
                            "}"
                            "}"
                            "}"
                            "}"
                            ).arg(buttonWidth).arg(buttonHeight)
                    .arg(m_imagePath);
        }


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
                isRowClosed = true;
            } else {
                isRowClosed = false;
            }

            r = r != maxRow - 1 ? r + 1 : 0;
        } else {
            if (isRowClosed)
                isRowClosed = false;
        }

        c = (c != maxCol - 1) ? c + 1 : 0;
    }

    if (c != 0) {
        page += "}";    // close the row
    }

    if (r != 0 || !isRowClosed) {
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

