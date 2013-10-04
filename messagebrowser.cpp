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

#include "pagemodel.hpp"
#include "rt.hpp"

#define     UI_FILE            "qrc:/ui/messagebrowser.qml"

using namespace std;
using namespace SMSDB;

#if defined(Q_OS_ANDROID)
MessageBrowser::MessageBrowser(const QString &category,
                               keyPressHandler_ptr keyPressHandler,
                               QWindow *parent) :
    Window(parent),
    m_keyPressHandler(keyPressHandler),
    m_hasBeenClosed(false)
#else
MessageBrowser::MessageBrowser(const QString &category,
                               QWindow *parent) :
    Window(parent)
#endif
{
    this->setTitle("پیامک بانک");
    this->setFlags(Qt::Window | Qt::FramelessWindowHint);

    m_pageModel = make_unique<PageModel>();

    FillMessagePages(category);

    for (const auto &p : m_pages) {
        m_pageModel->AddPage(p.get());
    }

    QQmlContext *context = this->rootContext();
    context->setContextProperty("PageModel", m_pageModel.get());

    this->SetQML(QStringLiteral(UI_FILE));
}

MessageBrowser::~MessageBrowser()
{
}

void MessageBrowser::shareMessage(QString message)
{
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

void MessageBrowser::FillMessagePages(const QString &category)
{
    QSqlQuery query(QString(" SELECT TRIM(message) AS msg_col "
                            " FROM messages "
                            " WHERE category = TRIM('%1') "
                            " GROUP BY msg_col "
                            " ORDER BY msg_col; ").arg(category));
    QSqlRecord record = query.record();

    const double padding = 96.0;
    QString page;

    const double textWidth = getScreenWidth() - (padding * 2.0);
    const double textHeight = getScreenHeight() - (padding * 2.0);

    size_t queryCount = -1;
    while(query.next()) {
        ++queryCount;
    }
    query.first();

#ifdef Q_OS_ANDROID
    QString imagePath("assets:/resources/img/");
#else
    QString imagePath("file:" + QDir::currentPath() + "/resources/img/");
#endif

    size_t i = 0;
    while (query.next()) {
        page.clear();

        QString message = query.value(record.indexOf("msg_col")).toString();

        page = QString("import QtQuick 2.1;"
                       "import QtQuick.Controls 1.0;"
                       "import QtQuick.Controls.Styles 1.0;"
                       "import QtQuick.Layouts 1.0;"
                       "Rectangle {"
                       "anchors.centerIn: parent;"
                       "anchors.fill: parent;"
                       "Rectangle {"
                       "anchors.centerIn: parent;"
                       "width: %1;"
                       "height: %2;"
                       "Rectangle {"
                       "width: parent.width;"
                       "Image {"
                       //"anchors.horizontalCenter: parent.horizontalCenter;"
                       "source: \"%3share_144x144.png\";"
                       "asynchronous: true;"
                       "cache: true;"
                       "smooth: true;"
                       "MouseArea {"
                       "anchors.fill: parent;"
                       "onClicked: {"
                       "cppWindow.shareMessage('%4')"
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
                       "text: \"%4\";"
                       "}"
                       "Text {"
                       "anchors.bottom: parent.bottom;"
                       "anchors.horizontalCenter: parent.horizontalCenter;"
                       "wrapMode: Text.NoWrap;"
                       "text: \"%5 / %6\";"
                       "}"
                       "}"
                       "}").arg(textWidth).arg(textHeight).arg(imagePath).arg(
                    message.replace("\"", "").replace("\n", "<br />"))
                .arg(++i).arg(queryCount);

        m_pages.push_back(make_unique<Page>(page));
    }
}

#if defined(Q_OS_ANDROID)
void MessageBrowser::Close()
{
    emit signal_Closed();
    this->hide();
    m_hasBeenClosed = true;
}

void MessageBrowser::keyPressEvent(QKeyEvent *e)
{
    if (e->key() != Qt::Key_MediaPrevious
            && e->key() != Qt::Key_Backspace) {
        QtQuick2ApplicationViewer::keyPressEvent(e);
    } else {
        if (!m_hasBeenClosed) {
            Close();
        } else {
            (this->*m_keyPressHandler)(e);
        }
    }
}
#endif

