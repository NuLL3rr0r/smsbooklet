#ifndef MAINWINDOW_HPP
#define MAINWINDOW_HPP


#include <memory>
#include <unordered_map>
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

    QString m_imagePath;

    std::unordered_map<Window::DisplayRatio,
    QString, Window::Hasher<Window::DisplayRatio>> m_splashScreenImages;

public:
    explicit MainWindow(QWindow *parent = 0);
    ~MainWindow();

private slots:
    void OnMessageBrowserClosed();
    void OnMessageBrowserShown();

public:
    Q_INVOKABLE void loadMessages(QString category);

private:
    void FillCategoryPages();
};


#endif // MAINWINDOW_HPP
