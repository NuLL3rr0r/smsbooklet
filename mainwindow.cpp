#include <string>
#include <cassert>
#include <cmath>
#include <QtCore/QDir>
#if !defined(Q_OS_ANDROID)
#include <QtCore/QMimeData>
#endif
#include <QtCore/QString>
#if !defined(Q_OS_ANDROID)
#include <QtGui/QClipboard>
#endif
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
#ifdef Q_OS_ANDROID
#include "android.hpp"
#endif
#include "db.hpp"
#include "dbtables.hpp"
#include "defs.hpp"
#include "localization.hpp"
#include "pagemodel.hpp"
#include "rt.hpp"

#define     UI_FILE_MAIN_WINDOW               "qrc:/ui/mainwindow.qml"

using namespace std;
using namespace SMSDB;

MainWindow::MainWindow(QWindow *parent) :
    Window(parent),
#ifdef Q_OS_ANDROID
    m_fontPath("assets:/resources/fnt/main.ttf"),
    m_imagePath("assets:/resources/img/"),
#else
    m_fontPath("file:" + QDir::currentPath() + "/resources/fnt/main.ttf"),
    m_imagePath("file:" + QDir::currentPath() + "/resources/img/"),
#endif
    m_splashScreenImages {
{Window::DisplayRatio::Vert_10_16, m_imagePath},
{Window::DisplayRatio::Vert_9_16, m_imagePath},
{Window::DisplayRatio::Vert_3_4, m_imagePath}
        },
    m_aboutPageBgImages {
{Window::DisplayRatio::Vert_10_16, m_imagePath + "about_bg_1200x1920.png"},
{Window::DisplayRatio::Vert_9_16, m_imagePath + "about_bg_1080x1920.png"},
{Window::DisplayRatio::Vert_3_4, m_imagePath + "about_bg_1440x1920.png"}
      }
{
    this->setTitle(APP_TITLE);
    this->setFlags(Qt::Window | Qt::FramelessWindowHint);

    double scrHeight = this->getScreenHeight();
    if (scrHeight > 1600.0) {
        m_splashScreenImages[Window::DisplayRatio::Vert_10_16] += "splashscreen_1200x1920.jpg";
        m_splashScreenImages[Window::DisplayRatio::Vert_9_16] += "splashscreen_1080x1920.jpg";
        m_splashScreenImages[Window::DisplayRatio::Vert_3_4] += "splashscreen_1440x1920.jpg";
    } else if (scrHeight > 1280.0) {
        m_splashScreenImages[Window::DisplayRatio::Vert_10_16] += "splashscreen_1000x1600.jpg";
        m_splashScreenImages[Window::DisplayRatio::Vert_9_16] += "splashscreen_900x1600.jpg";
        m_splashScreenImages[Window::DisplayRatio::Vert_3_4] += "splashscreen_1200x1600.jpg";
    } else if (scrHeight > 960.0) {
        m_splashScreenImages[Window::DisplayRatio::Vert_10_16] += "splashscreen_800x1280.jpg";
        m_splashScreenImages[Window::DisplayRatio::Vert_9_16] += "splashscreen_720x1280.jpg";
        m_splashScreenImages[Window::DisplayRatio::Vert_3_4] += "splashscreen_960x1280.jpg";
    } else if (scrHeight > 640.0) {
        m_splashScreenImages[Window::DisplayRatio::Vert_10_16] += "splashscreen_600x960.jpg";
        m_splashScreenImages[Window::DisplayRatio::Vert_9_16] += "splashscreen_540x960.jpg";
        m_splashScreenImages[Window::DisplayRatio::Vert_3_4] += "splashscreen_720x960.jpg";
    } else if (scrHeight > 480.0) {
        m_splashScreenImages[Window::DisplayRatio::Vert_10_16] += "splashscreen_400x640.jpg";
        m_splashScreenImages[Window::DisplayRatio::Vert_9_16] += "splashscreen_360x640.jpg";
        m_splashScreenImages[Window::DisplayRatio::Vert_3_4] += "splashscreen_480x640.jpg";
    } else {
        m_splashScreenImages[Window::DisplayRatio::Vert_10_16] += "splashscreen_300x480.jpg";
        m_splashScreenImages[Window::DisplayRatio::Vert_9_16] += "splashscreen_270x480.jpg";
        m_splashScreenImages[Window::DisplayRatio::Vert_3_4] += "splashscreen_360x480.jpg";
    }

    m_pageModelMainWindow = make_unique<PageModel>();
    m_pagesMainWindow.push_back(make_unique<Page>(
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

    for (const auto &p : m_pagesMainWindow) {
        m_pageModelMainWindow->AddPage(p.get());
    }

    QQmlContext *context = this->rootContext();
    context->setContextProperty("FontPath", m_fontPath);
    context->setContextProperty("MainWindowPageModel", m_pageModelMainWindow.get());

    m_pageModelSubCategoryBrowser.reset();
    m_pageModelSubCategoryBrowser = make_unique<PageModel>();
    context->setContextProperty("SubCategoryBrowserPageModel", m_pageModelSubCategoryBrowser.get());

    m_pageModelMessageBrowser.reset();
    m_pageModelMessageBrowser = make_unique<PageModel>();
    context->setContextProperty("MessageBrowserPageModel", m_pageModelMessageBrowser.get());

    context->setContextProperty("AboutWindowBackgroundPath", m_aboutPageBgImages[GetDisplayRatio()]);
    context->setContextProperty("AboutWindowAppLogoPath", m_imagePath + "smsbooklet-480x380.png");
    context->setContextProperty("AboutWindowBrandingLogoPath", m_imagePath + "approsia_144x50.png");

    this->SetQML(QStringLiteral(UI_FILE_MAIN_WINDOW));
}

MainWindow::~MainWindow()
{
}

void MainWindow::mainWindow_About()
{
    QVariant returnedValue;
    QMetaObject::invokeMethod(this->rootObject(), "showWindow",
                              Q_RETURN_ARG(QVariant, returnedValue),
                              Q_ARG(QVariant, "AboutWindow"));
}

void MainWindow::mainWindow_BrowseSubCategories(QString category)
{
    m_pagesSubCategoryBrowser.clear();
    m_pageModelSubCategoryBrowser.reset();
    m_pageModelSubCategoryBrowser = make_unique<PageModel>();

    FillSubCategoryPages(category);

    for (const auto &p : m_pagesSubCategoryBrowser) {
        m_pageModelSubCategoryBrowser->AddPage(p.get());
    }

    QQmlContext *context = this->rootContext();
    context->setContextProperty("SubCategoryBrowserPageModel", m_pageModelSubCategoryBrowser.get());

    QVariant returnedValue;

    QMetaObject::invokeMethod(this->rootObject(), "initSubCategoryBrowser");
    QMetaObject::invokeMethod(this->rootObject(), "showWindow",
                              Q_RETURN_ARG(QVariant, returnedValue),
                              Q_ARG(QVariant, "SubCategoryBrowser"));
}

void MainWindow::mainWindow_BrowseMessages(QString category)
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

    m_pagesMessageBrowser.clear();
    m_pageModelMessageBrowser.reset();
    m_pageModelMessageBrowser = make_unique<PageModel>();

    FillMessagePages(category);

    for (const auto &p : m_pagesMessageBrowser) {
        m_pageModelMessageBrowser->AddPage(p.get());
    }

    QQmlContext *context = this->rootContext();
    context->setContextProperty("MessageBrowserPageModel", m_pageModelMessageBrowser.get());

    QVariant returnedValue;

    QMetaObject::invokeMethod(this->rootObject(), "initMessageBrowser");
    QMetaObject::invokeMethod(this->rootObject(), "showWindow",
                              Q_RETURN_ARG(QVariant, returnedValue),
                              Q_ARG(QVariant, "MessageBrowser"));
}

void MainWindow::subCategoryBrowser_BrowseMessages(QString subCategory)
{
    m_pagesMessageBrowser.clear();
    m_pageModelMessageBrowser.reset();
    m_pageModelMessageBrowser = make_unique<PageModel>();

    FillMessagePages(subCategory);

    for (const auto &p : m_pagesMessageBrowser) {
        m_pageModelMessageBrowser->AddPage(p.get());
    }

    QQmlContext *context = this->rootContext();
    context->setContextProperty("MessageBrowserPageModel", m_pageModelMessageBrowser.get());

    QVariant returnedValue;

    QMetaObject::invokeMethod(this->rootObject(), "initMessageBrowser");
    QMetaObject::invokeMethod(this->rootObject(), "showWindow",
                              Q_RETURN_ARG(QVariant, returnedValue),
                              Q_ARG(QVariant, "MessageBrowser"));

    QSqlQuery query(QString(" SELECT lastpage "
                            " FROM subcategories "
                            " WHERE name = '%1'; ").arg(subCategory));
    if (query.next()) {
        QSqlRecord record = query.record();
        QString pageNum = query.value(record.indexOf("lastpage")).toString();
        if (pageNum.trimmed() != "") {
            QMetaObject::invokeMethod(this->rootObject(), "navigateToMessageBrowserPage",
                                      Q_ARG(QVariant, pageNum));
        } else {
            QMetaObject::invokeMethod(this->rootObject(), "navigateToMessageBrowserPage",
                                      Q_ARG(QVariant, 1));
        }
    } else {
        QMetaObject::invokeMethod(this->rootObject(), "navigateToMessageBrowserPage",
                                  Q_ARG(QVariant, 1));
    }
}

void MainWindow::messageBrowser_CopyToClipboard(QString message)
{
    message = message.replace("<br />", "\r\n")
            .replace("<br/>", "\r\n")
            .replace("<br>", "\r\n").remove(
                QRegExp( "<(?:div|span|tr|td|br|body|html|tt|a|strong|p)[^>]*>",
                         Qt::CaseInsensitive));

#if defined(Q_OS_ANDROID)
    RT::Android()->CopyToClipboard(message);
#else
    QMimeData *mimeData = new QMimeData();
    mimeData->setText(message);
    mimeData->setHtml(message);
    QClipboard *clipboard = QGuiApplication::clipboard();
    clipboard->setMimeData(mimeData, QClipboard::Clipboard);
#endif
}

void MainWindow::messageBrowser_ShareMessage(QString message)
{
    message = message.replace("<br />", "\r\n")
            .replace("<br/>", "\r\n")
            .replace("<br>", "\r\n").remove(
                QRegExp( "<(?:div|span|tr|td|br|body|html|tt|a|strong|p)[^>]*>",
                         Qt::CaseInsensitive));

#if defined(Q_OS_ANDROID)
    RT::Android()->SendText(message);
#else
    QMimeData *mimeData = new QMimeData();
    mimeData->setText(message);
    mimeData->setHtml(message);
    QClipboard *clipboard = QGuiApplication::clipboard();
    clipboard->setMimeData(mimeData, QClipboard::Clipboard);
#endif
}

void MainWindow::messageBrowser_ToggleFavourite(QString messageId)
{
    QSqlQuery query(QString(" SELECT fav "
                            " FROM messages "
                            " WHERE id = '%1'; ").arg(messageId));
    if (query.next()) {
        QSqlRecord record = query.record();
        bool isFavourite = query.value(record.indexOf("fav")).toBool();

        RT::DB()->Update(RT::DBTables()->Table(DBTables::TableName::Messages),
                         "id", messageId.toStdString(),
                         "fav=?", 1, isFavourite ? "0" : "1");
    }
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
    const double minPaddingX = 80.0;
    const double minPaddingY = 84.0;
    const unsigned char maxCol = std::floor(
                ((getScreenWidth() - (minPaddingX * 2.0)) + spacing) / (buttonWidth + spacing));
    const unsigned char maxRow = std::floor(
                ((getScreenHeight() - (minPaddingY * 2.0)) + spacing) / (buttonHeight + spacing));
    const double paddingW = (getScreenWidth() -
            ((buttonWidth * maxCol) + (spacing * (maxCol - 1)))) / 2.0;
    const double paddingH = (getScreenHeight() -
            ((buttonHeight * maxRow) + (spacing * (maxRow - 1)))) / 2.0;
    const double pageContentsHeight = getScreenHeight() - (paddingH * 2.0);
    const double pageContentsWidth = getScreenWidth() - (paddingW * 2.0);
    const double pageNumberMargin = (getScreenHeight() - pageContentsHeight) / 4.0;

    unsigned char r = 0;
    unsigned char c = 0;
    QString page;

    size_t queryCount = 0;
    size_t totalButtonsCount = 0;
    bool insertedAbout = false;
    bool insertedFav = false;

    while(query.next()) {
        ++queryCount;
    }
    queryCount += 2; // for 'About' and 'Favourite' buttons.
    totalButtonsCount = queryCount;
    query.first();
    query.previous();
    queryCount = ceil((double)queryCount / (double)(maxRow * maxCol));

    bool isRowClosed = false;
    size_t i = 0;
    for (size_t currentButtonIndex = 0; currentButtonIndex < totalButtonsCount; ++currentButtonIndex) {
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
                if (query.next()) {
                    category = query.value(record.indexOf("name_col")).toString();
                    icon = query.value(record.indexOf("icon")).toString();
                }
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
                    .arg(category != FAV_BUTTON_TEXT ? "mainWindow_BrowseSubCategories"
                                                     : "mainWindow_BrowseMessages");
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
                            "cppWindow.mainWindow_About();"
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
                                "font.family: textFont.name;"
                                "font.pointSize: 12.0;"
                                "}"
                                "}")            // close the rectangle
                        .arg(pageNumberMargin)
        #if !defined(Q_OS_ANDROID)
                        .arg(Localization::FormatNumsToPersian(std::to_string(++i).c_str()))
                        .arg(Localization::FormatNumsToPersian(std::to_string(queryCount).c_str()));
        #else
                        .arg(Localization::FormatNumsToPersian(QString::number(++i)))
                        .arg(Localization::FormatNumsToPersian(QString::number(queryCount)));
        #endif  // !defined(Q_OS_ANDROID)
                m_pagesMainWindow.push_back(make_unique<Page>(page));
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
                        "font.family: textFont.name;"
                        "font.pointSize: 12.0;"
                        "}"
                        "}") // close the rectangle
                .arg(pageNumberMargin)
#if !defined(Q_OS_ANDROID)
                .arg(Localization::FormatNumsToPersian(std::to_string(++i).c_str()))
                .arg(Localization::FormatNumsToPersian(std::to_string(queryCount).c_str()));
#else
                .arg(Localization::FormatNumsToPersian(QString::number(++i)))
                .arg(Localization::FormatNumsToPersian(QString::number(queryCount)));
#endif  // !defined(Q_OS_ANDROID)
        m_pagesMainWindow.push_back(make_unique<Page>(page));
    }
}

void MainWindow::FillSubCategoryPages(const QString &category)
{
    QSqlQuery query(QString(" SELECT TRIM ( name ) AS name_col, icon "
                    " FROM subcategories "
                    " WHERE catid "
                    " IN ( "
                    " SELECT id "
                    " FROM categories "
                    " WHERE name = '%1' "
                    " ) "
                    " GROUP BY name_col "
                    " ORDER BY name_col; ").arg(category));

    QSqlRecord record = query.record();

    const double buttonWidth = 144.0;
    const double buttonHeight = 144.0;
    const double spacing = 8.0;
    const double minPaddingX = 80.0;
    const double minPaddingY = 84.0;
    const unsigned char maxCol = std::floor(
                ((getScreenWidth() - (minPaddingX * 2.0)) + spacing) / (buttonWidth + spacing));
    const unsigned char maxRow = std::floor(
                ((getScreenHeight() - (minPaddingY * 2.0)) + spacing) / (buttonHeight + spacing));
    const double paddingW = (getScreenWidth() -
            ((buttonWidth * maxCol) + (spacing * (maxCol - 1)))) / 2.0;
    const double paddingH = (getScreenHeight() -
            ((buttonHeight * maxRow) + (spacing * (maxRow - 1)))) / 2.0;
    const double pageContentsHeight = getScreenHeight() - (paddingH * 2.0);
    const double pageContentsWidth = getScreenWidth() - (paddingW * 2.0);
    const double pageNumberMargin = (getScreenHeight() - pageContentsHeight) / 4.0;

    unsigned char r = 0;
    unsigned char c = 0;
    QString page;

    size_t queryCount = 0;
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

        QString subCategory = query.value(record.indexOf("name_col")).toString();
        QString icon = query.value(record.indexOf("icon")).toString();

        page += QString("Column {"
                        "Image {"
                        "source: '%4%5';"
                        "width: %2;"
                        "height: %3;"
                        "MouseArea {"
                        "anchors.fill: parent;"
                        "onClicked: {"
                        "cppWindow.subCategoryBrowser_BrowseMessages('%1');"
                        "}"
                        "}"
                        "}"
                        "}"
                        ).arg(subCategory).arg(buttonWidth).arg(buttonHeight)
                .arg(m_imagePath).arg(icon);

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
                                "font.family: textFont.name;"
                                "font.pointSize: 12.0;"
                                "}"
                                "}")            // close the rectangle
                        .arg(pageNumberMargin)
        #if !defined(Q_OS_ANDROID)
                        .arg(Localization::FormatNumsToPersian(std::to_string(++i).c_str()))
                        .arg(Localization::FormatNumsToPersian(std::to_string(queryCount).c_str()));
        #else
                        .arg(Localization::FormatNumsToPersian(QString::number(++i)))
                        .arg(Localization::FormatNumsToPersian(QString::number(queryCount)));
        #endif  // !defined(Q_OS_ANDROID)
                m_pagesSubCategoryBrowser.push_back(make_unique<Page>(page));
                isRowClosed = true;
            } else {
                if (isRowClosed)
                    isRowClosed = false;
            }

            r = r != maxRow - 1 ? r + 1 : 0;
        } else {
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
                        "font.family: textFont.name;"
                        "font.pointSize: 12.0;"
                        "}"
                        "}")            // close the rectangle
                .arg(pageNumberMargin)
#if !defined(Q_OS_ANDROID)
                .arg(Localization::FormatNumsToPersian(std::to_string(++i).c_str()))
                .arg(Localization::FormatNumsToPersian(std::to_string(queryCount).c_str()));
#else
                .arg(Localization::FormatNumsToPersian(QString::number(++i)))
                .arg(Localization::FormatNumsToPersian(QString::number(queryCount)));
#endif  // !defined(Q_OS_ANDROID)
        m_pagesSubCategoryBrowser.push_back(make_unique<Page>(page));
    }
}

void MainWindow::FillMessagePages(const QString &subCategory)
{
    m_currentSubCategory = subCategory;

    QSqlQuery query;

    if (subCategory != FAV_BUTTON_TEXT) {
        query.prepare(QString(" SELECT id, TRIM ( text ) AS text_col, fav "
                              " FROM messages "
                              " WHERE subcatid "
                              " IN ( "
                              " SELECT id "
                              " FROM subcategories "
                              " WHERE name = TRIM( '%1' ) "
                              " ) "
                              " GROUP BY text_col "
                              " ORDER BY text_col; ").arg(subCategory));
    } else {
        query.prepare(" SELECT id, TRIM ( text ) AS text_col, fav "
                      " FROM messages "
                      " WHERE fav = 1; ");
    }

    query.exec();
    QSqlRecord record = query.record();

    const double padding = getScreenWidth() / 16.0;
    const double navButtonSize = getScreenWidth() / 11.0;
    QString page;

    const double textWidth = getScreenWidth() - (padding * 2.0);
    const double textHeight = getScreenHeight() - (padding * 2.0);

    size_t queryCount = 0;
    while(query.next()) {
        ++queryCount;
    }
    query.first();
    query.previous();

#ifdef Q_OS_ANDROID
    QString imagePath("assets:/resources/img/");
#else
    QString imagePath("file:" + QDir::currentPath() + "/resources/img/");
#endif

    size_t i = 0;
    while (query.next()) {
        page.clear();

        QString messageId = query.value(record.indexOf("id")).toString();;
        QString message = query.value(record.indexOf("text_col")).toString();
        bool isFavourite = query.value(record.indexOf("fav")).toBool();

        ++i;
        page = QString("import QtQuick 2.1;"
                       "import QtQuick.Controls 1.0;"
                       "import QtQuick.Controls.Styles 1.0;"
                       "import QtQuick.Layouts 1.0;"
                       "import \"../PageScroll\";"
                       "Rectangle {"
                       "anchors.centerIn: parent;"
                       "anchors.fill: parent;"
                       "Rectangle {"
                       "anchors.centerIn: parent;"
                       "width: %1;"
                       "height: %2;"
                       "Rectangle {"
                       "id: navBar;"
                       "width: parent.width;"
                       "Row {"
                       "spacing: %4 / 2.5;"
                       "Column {"
                       "Image {"
                       "source: '%3copy-to-clipboard_144x144.png';"
                       "asynchronous: true;"
                       "cache: true;"
                       "smooth: true;"
                       "width: %4;"
                       "height: %4;"
                       "MouseArea {"
                       "anchors.fill: parent;"
                       "onClicked: {"
                       "cppWindow.messageBrowser_CopyToClipboard('%5');"
                       "notification.notificationText = '';"
                       "notification.notificationText = messageBrowser.msgCopiedToClipboard;"
                       "}"
                       "}"
                       "}"
                       "}"
                       "Column {"
                       "Image {"
                       "source: '%3share_144x144.png';"
                       "asynchronous: true;"
                       "cache: true;"
                       "smooth: true;"
                       "width: %4;"
                       "height: %4;"
                       "MouseArea {"
                       "anchors.fill: parent;"
                       "onClicked: {"
                       "cppWindow.messageBrowser_ShareMessage('%5');"
                       "}"
                       "}"
                       "}"
                       "}"
                       "Column {"
                       "Image {"
                       "id: 'favIcon';"
                       "source: '%3%6';"
                       "asynchronous: true;"
                       "cache: true;"
                       "smooth: true;"
                       "width: %4;"
                       "height: %4;"
                       "MouseArea {"
                       "anchors.fill: parent;"
                       "onClicked: {"
                       "cppWindow.messageBrowser_ToggleFavourite('%7');"
                       "if (favIcon.source == '%3%6') {"
                       "favIcon.source = '%3%8';"
                       "flipBook.model.set(0, { 'contents': flipBook.model.get(0).contents.replace('%3%6', '%3%8') });"
                       "notification.notificationText = %12;"
                       "} else {"
                       "favIcon.source = '%3%6';"
                       "flipBook.model.set(0, { 'contents': flipBook.model.get(0).contents.replace('%3%8', '%3%6') });"
                       "notification.notificationText = %13;"
                       "}"
                       "}"
                       "}"
                       "}"
                       "}"
                       "}"
                       "Column {"
                       "PageScroll {"
                       "x: ((%4 * 3.0) + ((%4 / 2.5) * 3.0));"
                       "width: navBar.width - x;"
                       "height: %4;"
                       "barImageSource: '%3bar_669x14.png';"
                       "barImageW: width;"
                       "barImageH: %4 / 144.0 * 14.0;"
                       "knobImageSource: '%3knob_144x144.png';"
                       "knobImageW: %4;"
                       "knobImageH: %4;"
                       "minValue: 1.0;"
                       "maxValue: %15;"
                       "defaultValue: %14;"
                       "onSignal_pressed: {"
                       "pageNumber.text = formatNumsToPersian(String(Math.round(progress))) + ' / %11';"
                       "}"
                       "onSignal_released: {"
                       "navigateToMessageBrowserPage(Math.round(progress));"
                       "}"
                       "onSignal_positionChanged: {"
                       "pageNumber.text = formatNumsToPersian(String(Math.round(progress))) + ' / %11';"
                       "}"
                       "}"
                       "}"
                       "}"
                       "Text {"
                       "anchors.centerIn: parent;"
                       "anchors.fill: parent;"
                       "verticalAlignment: Text.AlignVCenter;"
                       "horizontalAlignment: Text.AlignHCenter;"
                       "wrapMode: Text.WordWrap;"
                       "font.family: textFont.name;"
                       "font.pointSize: 12.0;"
                       "text: '<h3>%9</h3><br />%5';"
                       "LayoutMirroring.enabled: true;"
                       "LayoutMirroring.childrenInherit: true;"
                       "}"
                       "Text {"
                       "id: pageNumber;"
                       "anchors.bottom: parent.bottom;"
                       "anchors.horizontalCenter: parent.horizontalCenter;"
                       "wrapMode: Text.NoWrap;"
                       "font.family: textFont.name;"
                       "font.pointSize: 12.0;"
                       "text: '%10 / %11';"
                       "}"
                       "}"
                       "}").arg(textWidth).arg(textHeight).arg(imagePath)
                .arg(navButtonSize).arg(message.replace("\"",
                                                        QString("%1%2")
                                                        .arg("\\").arg("\""))
                                        .replace("'", "\\'")
                                        .replace("\n", "<br />"))
                .arg(!isFavourite ? "favourite_unselected_144x144.png" : "favourite_selected_144x144.png")
                .arg(messageId)
                .arg(!isFavourite ? "favourite_selected_144x144.png" : "favourite_unselected_144x144.png")
                .arg(Localization::FormatNumsToPersian(subCategory))
#if !defined(Q_OS_ANDROID)
                .arg(Localization::FormatNumsToPersian(std::to_string(i).c_str()))
                .arg(Localization::FormatNumsToPersian(std::to_string(queryCount).c_str()))
#else
                .arg(Localization::FormatNumsToPersian(QString::number(i)))
                .arg(Localization::FormatNumsToPersian(QString::number(queryCount)))
#endif  // !defined(Q_OS_ANDROID)
        .arg(!isFavourite ? "messageBrowser.msgAddedToFav" : "messageBrowser.msgRemovedFromFav")
                .arg(!isFavourite ? "messageBrowser.msgRemovedFromFav" : "messageBrowser.msgAddedToFav")
                .arg(i).arg(queryCount);

        m_pagesMessageBrowser.push_back(make_unique<Page>(page));
    }
}

void MainWindow::keyPressEvent(QKeyEvent *e)
{
    if (e->key() != Qt::Key_MediaPrevious
            && e->key() != Qt::Key_Backspace) {
        Window::keyPressEvent(e);
    } else {
        QVariant returnedValue;

        QMetaObject::invokeMethod(this->rootObject(), "getCurrentWindowId",
                                  Q_RETURN_ARG(QVariant, returnedValue));

        string currentWindowId = returnedValue.toString().toStdString();
        if (currentWindowId == "MainWindow") {
            Window::keyPressEvent(e);
        } else if (currentWindowId == "SubCategoryBrowser") {
            QMetaObject::invokeMethod(this->rootObject(), "showWindow",
                                      Q_RETURN_ARG(QVariant, returnedValue),
                                      Q_ARG(QVariant, "MainWindow"));
        } else if (currentWindowId == "MessageBrowser") {
            QMetaObject::invokeMethod(this->rootObject(), "getMessageBrowserCurrentPageNumber",
                                      Q_RETURN_ARG(QVariant, returnedValue));

            if (m_currentSubCategory != FAV_BUTTON_TEXT) {
                RT::DB()->Update(RT::DBTables()->Table(DBTables::TableName::SubCategories),
                                 "name", m_currentSubCategory.toStdString(),
                                 "lastpage=?", 1, returnedValue.toString().toStdString().c_str());

                QMetaObject::invokeMethod(this->rootObject(), "showWindow",
                                          Q_RETURN_ARG(QVariant, returnedValue),
                                          Q_ARG(QVariant, "SubCategoryBrowser"));
            } else {
                QMetaObject::invokeMethod(this->rootObject(), "showWindow",
                                          Q_RETURN_ARG(QVariant, returnedValue),
                                          Q_ARG(QVariant, "MainWindow"));
            }
        } else if (currentWindowId == "AboutWindow") {
            QMetaObject::invokeMethod(this->rootObject(), "showWindow",
                                      Q_RETURN_ARG(QVariant, returnedValue),
                                      Q_ARG(QVariant, "MainWindow"));
        }
    }
}

