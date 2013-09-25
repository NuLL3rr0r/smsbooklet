#ifndef WINDOW_HPP
#define WINDOW_HPP


#include <memory>
#include "qtquick2applicationviewer.h"

namespace SMSDB {
    class Window;
}

class SMSDB::Window : public QtQuick2ApplicationViewer
{
    Q_OBJECT

private:
    class Pimpl;
    std::unique_ptr<Pimpl> m_pimpl;

public:
    explicit Window(QWindow *parent = NULL,
                    Qt::WindowType flags = Qt::FramelessWindowHint);
    virtual ~Window();

signals:
    void signal_Shown();
    void signal_Closed();

protected:
    void keyPressEvent(QKeyEvent *e);

public:
    Q_INVOKABLE double getScreenWidth() const;
    Q_INVOKABLE double getScreenHeight() const;

public:
    void SetQML(const QString &url);

public:
    virtual void Show();
    virtual void Close();
};


#endif // WINDOW_HPP

