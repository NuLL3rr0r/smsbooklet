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
#include "messagebrowser.hpp"
#if defined(Q_OS_ANDROID)
#include "android.hpp"
#endif
#include "db.hpp"
#include "dbtables.hpp"
#include "defs.hpp"
#include "localization.hpp"
#include "pagemodel.hpp"
#include "rt.hpp"

#define     UI_FILE            "qrc:/ui/messagebrowser.qml"

using namespace std;
using namespace SMSDB;

#if defined(Q_OS_ANDROID)
MessageBrowser::MessageBrowser(const QString &category,
                               keyPressHandler_ptr keyPressHandler,
                               QWindow *parent) :
#else
MessageBrowser::MessageBrowser(const QString &category,
                               QWindow *parent) :
#endif
    Window(parent),
    m_category (category),
#if defined(Q_OS_ANDROID)
    m_keyPressHandler(keyPressHandler),
    m_hasBeenClosed(false),
    m_fontPath("assets:/resources/fnt/main.ttf"),
    m_imagePath("assets:/resources/img/")
#else
    m_fontPath("file:" + QDir::currentPath() + "/resources/fnt/main.ttf"),
    m_imagePath("file:" + QDir::currentPath() + "/resources/img/")
#endif
{
    this->setTitle(APP_TITLE);
    this->setFlags(Qt::Window | Qt::FramelessWindowHint);

    m_pageModel = make_unique<PageModel>();

    FillMessagePages(category);

    for (const auto &p : m_pages) {
        m_pageModel->AddPage(p.get());
    }

    QQmlContext *context = this->rootContext();
    context->setContextProperty("FontPath", m_fontPath);
    context->setContextProperty("PageModel", m_pageModel.get());

    this->SetQML(QStringLiteral(UI_FILE));

    QSqlQuery query(QString(" SELECT lastpage "
                            " FROM subcategories "
                            " WHERE name = '%1'; ").arg(m_category));
    if (query.next()) {
        QSqlRecord record = query.record();
        QString pageNum = query.value(record.indexOf("lastpage")).toString();
        if (pageNum.trimmed() != "") {
            QMetaObject::invokeMethod(this->rootObject(), "navigateToPage",
                                      Q_ARG(QVariant, pageNum));
        }
    }
}

MessageBrowser::~MessageBrowser()
{
}

void MessageBrowser::copyToClipboard(QString message)
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

void MessageBrowser::shareMessage(QString message)
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

void  MessageBrowser::toggleFavourite(QString messageId)
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

void MessageBrowser::FillMessagePages(const QString &subCategory)
{
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
                       "cppWindow.copyToClipboard('%5');"
                       "notification.notificationText = '';"
                       "notification.notificationText = msgCopiedToClipboard;"
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
                       "cppWindow.shareMessage('%5');"
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
                       "cppWindow.toggleFavourite('%7');"
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
                       "flipBook.navigateToPage(Math.round(progress));"
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
                .arg(Localization::FormatNumsToPersian(m_category))
#if !defined(Q_OS_ANDROID)
                .arg(Localization::FormatNumsToPersian(std::to_string(++i).c_str()))
                .arg(Localization::FormatNumsToPersian(std::to_string(queryCount).c_str()))
#else
                .arg(Localization::FormatNumsToPersian(QString::number(++i)))
                .arg(Localization::FormatNumsToPersian(QString::number(queryCount)))
#endif  // !defined(Q_OS_ANDROID)
        .arg(!isFavourite ? "msgAddedToFav" : "msgRemovedFromFav")
                .arg(!isFavourite ? "msgRemovedFromFav" : "msgAddedToFav")
                .arg(i).arg(queryCount);

        m_pages.push_back(make_unique<Page>(page));
    }
}

void MessageBrowser::Close()
{
    QVariant returnedValue;
    QMetaObject::invokeMethod(this->rootObject(), "getCurrentPageNumber",
                              Q_RETURN_ARG(QVariant, returnedValue));

    RT::DB()->Update(RT::DBTables()->Table(DBTables::TableName::SubCategories),
                     "name", m_category.toStdString(),
                     "lastpage=?", 1, returnedValue.toString().toStdString().c_str());

#if !defined(Q_OS_ANDROID)
    Window::Close();
#else
    emit signal_Closed();
    this->hide();
    m_hasBeenClosed = true;
#endif
}

#if defined(Q_OS_ANDROID)
void MessageBrowser::keyPressEvent(QKeyEvent *e)
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

