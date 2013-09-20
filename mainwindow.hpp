#ifndef MAINWINDOW_HPP
#define MAINWINDOW_HPP


#include <memory>
#include <vector>
#include "window.hpp"

class QString;

namespace SMSDB {
    class MainWindow;
    class MessageBrowser;
    class PageModel;
    class Page;
}

class SMSDB::MainWindow : public Window
{
    Q_OBJECT

private:
    std::unique_ptr<SMSDB::PageModel> m_pageModel;
    std::vector<std::unique_ptr<SMSDB::Page>> m_pages;
    std::unique_ptr<SMSDB::MessageBrowser> m_messageBrowser;

public:
    explicit MainWindow(QWindow *parent = 0);
    ~MainWindow();

public:
    Q_INVOKABLE void loadMessages(QString category);

private:
    void FillCategoryPages();
};


#endif // MAINWINDOW_HPP
