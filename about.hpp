#ifndef ABOUT_HPP
#define ABOUT_HPP


#include "window.hpp"

namespace SMSDB {
    class About;
}

class SMSDB::About : public Window
{
    Q_OBJECT

private:
#if defined(Q_OS_ANDROID)
    typedef std::function<void(QKeyEvent *)> keyPressHandler_ptr;
#endif

private:
#if defined(Q_OS_ANDROID)
    keyPressHandler_ptr m_keyPressHandler;
    bool m_hasBeenClosed;
#endif

    QString m_imagePath;

public:
#if defined(Q_OS_ANDROID)
    explicit About(keyPressHandler_ptr keyPressHandler, QWindow *parent = 0);
#else
    explicit About(QWindow *parent = 0);
#endif
    ~About();

#if defined(Q_OS_ANDROID)
public:
    void Close();

protected:
    void keyPressEvent(QKeyEvent *e);
#endif
};


#endif // ABOUT_HPP

