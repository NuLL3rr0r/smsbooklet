#ifndef MESSAGEBROWSER_HPP
#define MESSAGEBROWSER_HPP


#include <functional>
#include <memory>
#include <vector>
#include "window.hpp"

class QString;

namespace SMSDB {
    class MessageBrowser;
    class PageModel;
    class Page;
}

class SMSDB::MessageBrowser : public Window
{
    Q_OBJECT

private:
#if defined(Q_OS_ANDROID)
    typedef std::function<void(QKeyEvent *)> keyPressHandler_ptr;
#endif

private:
    QString m_category;
    std::unique_ptr<SMSDB::PageModel> m_pageModel;
    std::vector<std::unique_ptr<SMSDB::Page>> m_pages;

#if defined(Q_OS_ANDROID)
    keyPressHandler_ptr m_keyPressHandler;
    bool m_hasBeenClosed;
#endif

    QString m_fontPath;
    QString m_imagePath;

public:
#if defined(Q_OS_ANDROID)
    explicit MessageBrowser(const QString &category,
                            keyPressHandler_ptr keyPressHandler,
                            QWindow *parent = 0);
#else
    explicit MessageBrowser(const QString &category, QWindow *parent = 0);
#endif
    ~MessageBrowser();

public:
    Q_INVOKABLE void shareMessage(QString message);
    Q_INVOKABLE void toggleFavourite(QString messageId);

private:
    void FillMessagePages(const QString &subCategory);

#if defined(Q_OS_ANDROID)
public:
    void Close();

protected:
    void keyPressEvent(QKeyEvent *e);
#endif
};


#endif // MESSAGEBROWSER_HPP

