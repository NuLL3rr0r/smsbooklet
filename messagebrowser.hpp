#ifndef MESSAGEBROWSER_HPP
#define MESSAGEBROWSER_HPP


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
    std::unique_ptr<SMSDB::PageModel> m_pageModel;
    std::vector<std::unique_ptr<SMSDB::Page>> m_pages;

public:
    explicit MessageBrowser(const QString &category, QWindow *parent = 0);
    ~MessageBrowser();

public:
    Q_INVOKABLE void shareMessage(QString message);

private:
    void FillMessagePages(const QString &category);

public:
    //void Close();
};


#endif // MAINWINDOW_HPP

