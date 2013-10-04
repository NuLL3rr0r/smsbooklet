#include <unordered_map>
#include <cmath>
#include <QtCore/QUrl>
#include <QtGui/QGuiApplication>
#include <QtGui/QScreen>
#include <QtQml/QQmlContext>
#include <QtQuick/QQuickItem>
#include "make_unique.hpp"
#include "window.hpp"

#define     DEFAULT_WIDTH      480.0;
#define     DEFAULT_HEIGHT     768.0;

using namespace std;
using namespace SMSDB;

class Window::Pimpl {
private:
    Window *m_this;

public:
    unordered_map<Window::DisplayRatio,
    float, Window::Hasher<Window::DisplayRatio>> DisplayRatios {
        {Window::DisplayRatio::Horz_16_10, 1.6f},
        {Window::DisplayRatio::Horz_16_9, 1.777777777777778f},
        {Window::DisplayRatio::Horz_4_3, 1.333333333333333f},
        {Window::DisplayRatio::Vert_10_16, 0.625f},
        {Window::DisplayRatio::Vert_9_16, 0.5625f},
        {Window::DisplayRatio::Vert_3_4, 0.75f}
    };

public:
    Pimpl(Window *parent);

public:
    void ReLocate();
};

Window::Window(QWindow *parent, Qt::WindowType flags) :
    QtQuick2ApplicationViewer(parent),
    m_pimpl(std::make_unique<Window::Pimpl>(this))
{
    this->setFlags(flags);

    this->rootContext()->setContextProperty("cppWindow", this);
}

Window::~Window()
{
}

double Window::getScreenWidth() const
{
#if defined(Q_OS_ANDROID) || defined(Q_OS_BLACKBERRY)
    return QGuiApplication::primaryScreen()->size().width();
#else
    return DEFAULT_WIDTH;
#endif
}

double Window::getScreenHeight() const
{
#if defined(Q_OS_ANDROID) || defined(Q_OS_BLACKBERRY)
    return QGuiApplication::primaryScreen()->size().height();
#else
    return DEFAULT_HEIGHT;
#endif
}

void Window::SetQML(const QString &url)
{
    this->setSource(QUrl(url));
    m_pimpl->ReLocate();
}

Window::DisplayRatio Window::GetDisplayRatio()
{
    Window::DisplayRatio result;

    float actualRatio = getScreenWidth() / getScreenHeight();

    float minDiff = 10.0f;

    for (auto const &kv : m_pimpl->DisplayRatios) {
        float diff = abs(actualRatio - kv.second);
        if (diff < minDiff) {
            minDiff = diff;
            result = kv.first;
        }
    }

    return result;
}

void Window::Show()
{
    this->showExpanded();
    emit signal_Shown();
}

void Window::Close()
{
    // QML has a bug on Android,
    // so emit the signal first
    emit signal_Closed();
    this->close();
}

Window::Pimpl::Pimpl(Window *parent) :
    m_this(parent)
{
}

void Window::Pimpl::ReLocate()
{
    QSize screenSize = QGuiApplication::primaryScreen()->size();
    m_this->setX((screenSize.width() - m_this->width()) / 2.0);
    m_this->setY((screenSize.height() - m_this->height()) / 2.0);
}

void Window::keyPressEvent(QKeyEvent *e)
{
    if (e->key() != Qt::Key_MediaPrevious
            && e->key() != Qt::Key_Backspace) {
        QtQuick2ApplicationViewer::keyPressEvent(e);
    } else {
        this->Close();
    }
}

void Window::resizeEvent(QResizeEvent *e) {
    (void)e;

    // ToDo: Handle screen-rotation here

    QtQuick2ApplicationViewer::resizeEvent(e);
}

