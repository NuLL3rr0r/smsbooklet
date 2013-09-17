#ifndef MAINWINDOW_HPP
#define MAINWINDOW_HPP


#include <memory>
#include <vector>
#include "window.hpp"

namespace SMSDB {
    class MainWindow;
    class PageModel;
    class Page;
}

class SMSDB::MainWindow : public Window
{
    Q_OBJECT

private:
    std::unique_ptr<SMSDB::PageModel> m_pageModel;
    std::vector<std::unique_ptr<SMSDB::Page>> m_pages;

public:
    explicit MainWindow(QWindow *parent = 0);
    ~MainWindow();

private:
    void FillCategoryPages();
};


#endif // MAINWINDOW_HPP
