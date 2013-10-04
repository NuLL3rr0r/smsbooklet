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
#include "pagemodel.hpp"
#include "rt.hpp"

#define     UI_FILE            "qrc:/ui/subcategorybrowser.qml"

using namespace std;
using namespace SMSDB;

#if defined(Q_OS_ANDROID)
SubCategoryBrowser::SubCategoryBrowser(const QString &category,
                                       keyPressHandler_ptr keyPressHandler,
                                       QWindow *parent) :
    Window(parent),
    m_category (category),
    m_keyPressHandler(keyPressHandler),
    m_hasBeenClosed(false)
#else
SubCategoryBrowser::SubCategoryBrowser(const QString &category,
                                       QWindow *parent) :
    Window(parent),
    m_category (category)
#endif
{
    this->setTitle("پیامک بانک");
    this->setFlags(Qt::Window | Qt::FramelessWindowHint);

    m_pageModel = make_unique<PageModel>();

    FillSubCategoryPages(category);

    for (const auto &p : m_pages) {
        m_pageModel->AddPage(p.get());
    }

    QQmlContext *context = this->rootContext();
    context->setContextProperty("PageModel", m_pageModel.get());

    this->SetQML(QStringLiteral(UI_FILE));
}

SubCategoryBrowser::~SubCategoryBrowser()
{
}

void SubCategoryBrowser::browseMessages(QString subCategory)
{
    (void)subCategory;
    // ToDo:
}

void SubCategoryBrowser::FillSubCategoryPages(const QString &category)
{
    (void)category;
    // ToDo:
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
    if (e->key() != Qt::Key_MediaPrevious
            && e->key() != Qt::Key_Backspace) {
        if (!m_hasBeenClosed) {
            QtQuick2ApplicationViewer::keyPressEvent(e);
        } else {
            (this->*m_keyPressHandler)(e);
        }
    } else {
        if (!m_hasBeenClosed) {
            Close();
        } else {
            (this->*m_keyPressHandler)(e);
        }
    }
}
#endif

