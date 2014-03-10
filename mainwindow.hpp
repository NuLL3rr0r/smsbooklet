#ifndef MAINWINDOW_HPP
#define MAINWINDOW_HPP


#include <memory>
#include <unordered_map>
#include <vector>
#include "window.hpp"

class QString;

namespace SMSDB {
    class About;
    class MainWindow;
    class MessageBrowser;
    class PageModel;
    class Page;
    class SubCategoryBrowser;
}

class SMSDB::MainWindow : public Window
{
    Q_OBJECT

private:
    typedef std::unique_ptr<SMSDB::PageModel> PageModelPtr_t;
    typedef std::vector<std::unique_ptr<SMSDB::Page>> Pages_t;

private:
    PageModelPtr_t m_pageModelMainWindow;
    PageModelPtr_t m_pageModelSubCategoryBrowser;
    PageModelPtr_t m_pageModelMessageBrowser;

    Pages_t m_pagesMainWindow;
    Pages_t m_pagesSubCategoryBrowser;
    Pages_t m_pagesMessageBrowser;

    QString m_fontPath;
    QString m_imagePath;
    std::unordered_map<Window::DisplayRatio,
    QString, Window::Hasher<Window::DisplayRatio>> m_splashScreenImages;
    std::unordered_map<Window::DisplayRatio,
    QString, Window::Hasher<Window::DisplayRatio>> m_aboutPageBgImages;

    QString m_currentSubCategory;

public:
    explicit MainWindow(QWindow *parent = NULL);
    ~MainWindow();

public:
    Q_INVOKABLE void mainWindow_About();
    Q_INVOKABLE void mainWindow_BrowseSubCategories(QString category);
    Q_INVOKABLE void mainWindow_BrowseMessages(QString category);
    Q_INVOKABLE void subCategoryBrowser_BrowseMessages(QString subCategory);
    Q_INVOKABLE void messageBrowser_CopyToClipboard(QString message);
    Q_INVOKABLE void messageBrowser_ShareMessage(QString message);
    Q_INVOKABLE void messageBrowser_ToggleFavourite(QString messageId);

private:
    void FillCategoryPages();
    void FillSubCategoryPages(const QString &category);
    void FillMessagePages(const QString &subCategory);

protected:
    void keyPressEvent(QKeyEvent *e);
};


#endif // MAINWINDOW_HPP
