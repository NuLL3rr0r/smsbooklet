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
#include "messagebrowser.hpp"

#if defined(Q_OS_ANDROID)
#include "android.hpp"
#endif

#include "pagemodel.hpp"
#include "rt.hpp"

#define     UI_FILE            "qrc:/ui/messagebrowser.qml"

using namespace std;
using namespace SMSDB;

MessageBrowser::MessageBrowser(const QString &category, QWindow *parent) :
    Window(parent)
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
    (void)message;
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

    double textWidth = getScreenWidth() - (padding * 2.0);
    double textHeight = getScreenHeight() - (padding * 2.0);

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
                       "source: \"../share.png\";"
                       "asynchronous: true;"
                       "cache: true;"
                       "smooth: true;"
                       "MouseArea {"
                       "anchors.fill: parent;"
                       "onClicked: {"
                       "cppWindow.shareMessage('%3')"
                       "}"
                       "}"
                       "}"
                       "}"
                       "Text {"
                       "anchors.centerIn: parent;"
                       "anchors.fill: parent;"
                       "verticalAlignment: Text.AlignVCenter;"
                       "horizontalAlignment: Text.AlignHCenter;"
                       "wrapMode: 'Wrap';"
                       "text: \"%3\";"
                       "}"
                       "}"
                       "}").arg(textWidth).arg(textHeight).arg(
                    message.replace("\"", "").replace("\n", "<br />"));

        m_pages.push_back(make_unique<Page>(page));
    }
}

/*
// We should ovveride this, or else we crash.
// Use hide instead of close.
// We'll remove this object on parent
void MessageBrowser::Close()
{
    emit signal_Closed();
    this->hide();
}
*/
