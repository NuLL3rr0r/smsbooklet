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

protected:
    template <typename _T>
    struct Hasher
    {
        std::size_t operator()(const _T &t) const
        {
            return std::hash<unsigned char>()(static_cast<unsigned char>(t));
        }
    };

    enum class DisplayRatio : unsigned char {
        Horz_16_10,
        Horz_16_9,
        Horz_4_3,
        Vert_10_16,
        Vert_9_16,
        Vert_3_4
    };

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

public:
    Q_INVOKABLE double getScreenWidth() const;
    Q_INVOKABLE double getScreenHeight() const;

public:
    void SetQML(const QString &url);
    DisplayRatio GetDisplayRatio();

public:
    virtual void Show();
    virtual void Close();

protected:
    void keyPressEvent(QKeyEvent *e);
    void resizeEvent(QResizeEvent *e);
};


#endif // WINDOW_HPP

