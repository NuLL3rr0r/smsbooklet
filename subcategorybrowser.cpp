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
#include "subcategorybrowser.hpp"
#include "defs.hpp"
#include "messagebrowser.hpp"
#include "pagemodel.hpp"
#include "rt.hpp"

#define     UI_FILE            "qrc:/ui/subcategorybrowser.qml"

using namespace std;
using namespace SMSDB;

#if defined(Q_OS_ANDROID)
SubCategoryBrowser::SubCategoryBrowser(const QString &category,
                                       keyPressHandler_ptr keyPressHandler,
                                       QWindow *parent) :
#else
SubCategoryBrowser::SubCategoryBrowser(const QString &category,
                                       QWindow *parent) :
#endif
    Window(parent),
    m_category (category),
#if defined(Q_OS_ANDROID)
    m_keyPressHandler(keyPressHandler),
    m_hasBeenClosed(false),
    m_fontPath("assets:/resources/fnt/BYekan.ttf"),
    m_imagePath("assets:/resources/img/")
#else
    m_fontPath("file:" + QDir::currentPath() + "/resources/fnt/BYekan.ttf"),
    m_imagePath("file:" + QDir::currentPath() + "/resources/img/")
#endif
{
    this->setTitle(APP_TITLE);
    this->setFlags(Qt::Window | Qt::FramelessWindowHint);

    m_pageModel = make_unique<PageModel>();

    FillSubCategoryPages(category);

    for (const auto &p : m_pages) {
        m_pageModel->AddPage(p.get());
    }

    QQmlContext *context = this->rootContext();
    context->setContextProperty("FontPath", m_fontPath);
    context->setContextProperty("PageModel", m_pageModel.get());

    this->SetQML(QStringLiteral(UI_FILE));
}

SubCategoryBrowser::~SubCategoryBrowser()
{
}

void SubCategoryBrowser::OnMessageBrowserClosed() {
    this->setVisible(true);
#if !defined(Q_OS_ANDROID)
    m_messageBrowser.reset();
#endif
}

void SubCategoryBrowser::OnMessageBrowserShown() {
    this->setVisible(false);
}

void SubCategoryBrowser::browseMessages(QString subCategory)
{
#if defined(Q_OS_ANDROID)
    m_messageBrowser.reset();
    m_messageBrowser = make_unique<MessageBrowser>(subCategory,
                                                   std::bind(
                                                       &SubCategoryBrowser::keyPressEvent,
                                                       this,
                                                       std::placeholders::_1));
#else
    m_messageBrowser = make_unique<MessageBrowser>(subCategory);
#endif
    QObject::connect(m_messageBrowser.get(), SIGNAL(signal_Closed()),
                     this, SLOT(OnMessageBrowserClosed()));
    QObject::connect(m_messageBrowser.get(), SIGNAL(signal_Shown()),
                     this, SLOT(OnMessageBrowserShown()));
    m_messageBrowser->Show();
}

void SubCategoryBrowser::FillSubCategoryPages(const QString &category)
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
                        "cppWindow.browseMessages('%1');"
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
                                "}"
                                "}").arg(pageNumberMargin)
                        .arg(++i).arg(queryCount);    // close the rectangle
                m_pages.push_back(make_unique<Page>(page));
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
                        "}"
                        "}").arg(pageNumberMargin)
                .arg(++i).arg(queryCount);    // close the rectangle
        m_pages.push_back(make_unique<Page>(page));
    }
}

#if defined(Q_OS_ANDROID)
void SubCategoryBrowser::Close()
{
    emit signal_Closed();
    this->hide();
    m_hasBeenClosed = true;
}

void SubCategoryBrowser::keyPressEvent(QKeyEvent *e)
{
    if (e->key() != Qt::Key_MediaPrevious) {
        if (!m_hasBeenClosed) {
            QtQuick2ApplicationViewer::keyPressEvent(e);
        } else {
            this->m_keyPressHandler(e);
        }
    } else {
        if (!m_hasBeenClosed) {
            Close();
        } else {
            this->m_keyPressHandler(e);
        }
    }
}
#endif

