#ifndef SUBCATEGORYBROWSER_HPP
#define SUBCATEGORYBROWSER_HPP


#include <memory>
#include <vector>
#include "window.hpp"

class QString;

namespace SMSDB {
    class MessageBrowser;
    class PageModel;
    class Page;
    class SubCategoryBrowser;
}

class SMSDB::SubCategoryBrowser : public Window
{
    Q_OBJECT

private:
    typedef void (SMSDB::Window::*keyPressHandler_ptr)(QKeyEvent *);

private:
    QString m_category;
    std::unique_ptr<SMSDB::PageModel> m_pageModel;
    std::vector<std::unique_ptr<SMSDB::Page>> m_pages;
    std::unique_ptr<SMSDB::MessageBrowser> m_messageBrowser;

    keyPressHandler_ptr m_keyPressHandler;
#if defined(Q_OS_ANDROID)
    bool m_hasBeenClosed;
#endif

    QString m_imagePath;

public:
#if defined(Q_OS_ANDROID)
    explicit SubCategoryBrowser(const QString &category, keyPressHandler_ptr keyPressHandler,
                                QWindow *parent = 0);
#else
    explicit SubCategoryBrowser(const QString &category, QWindow *parent = 0);
#endif
    ~SubCategoryBrowser();

private slots:
    void OnMessageBrowserClosed();
    void OnMessageBrowserShown();

public:
    Q_INVOKABLE void browseMessages(QString subCategory);

private:
    void FillSubCategoryPages(const QString &category);

#if defined(Q_OS_ANDROID)
public:
    void Close();

protected:
    void keyPressEvent(QKeyEvent *e);
#endif
};


#endif // SUBCATEGORYBROWSER_HPP

