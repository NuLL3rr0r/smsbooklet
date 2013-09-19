#include <QtGui/QGuiApplication>
#include <QtGui/QScreen>
#include <QtQml/QQmlContext>
#include <QtQuick/QQuickItem>
#include "make_unique.hpp"
#include "window.hpp"

#define     DEFAULT_WIDTH      480.0;
#define     DEFAULT_HEIGHT     768.0;

using namespace SMSDB;

class Window::Pimpl {
private:
    Window *m_this;

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

void Window::SetQML(const QString &file)
{
    setMainQmlFile(file);
    m_pimpl->ReLocate();
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

