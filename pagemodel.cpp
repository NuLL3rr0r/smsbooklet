#include <QDebug>
#include "pagemodel.hpp"

using namespace SMSDB;

Page::Page(const QString &contents) :
    m_contents(contents)
{
}

Page::~Page()
{
}

QString Page::Contents() const
{
    return m_contents;
}

void Page::SetContents(const QString &contents)
{
    m_contents = contents;
}

PageModel::PageModel(QObject *parent) :
    QAbstractListModel(parent)
{
}

PageModel::~PageModel()
{
}

void PageModel::clear()
{
    m_pages.clear();
}

QVariantMap PageModel::get(int index)
{
    QVariantMap element;

    if (index >= this->m_pages.size() || index < 0)
        return element;

    Page *item = m_pages.at(index);
    element["contents"] = item->Contents();
    return element;
}

void PageModel::remove(int index)
{
    m_pages.removeAt(index);
}

void PageModel::set(int index, QVariantMap dict)
{
    Page *item = m_pages.at(index);
    item->SetContents(dict["contents"].toString());
}

void PageModel::AddPage(Page *page)
{
    beginInsertRows(QModelIndex(), rowCount(), rowCount());
    m_pages << page;
    endInsertRows();
}

int PageModel::rowCount(const QModelIndex &parent) const
{
    (void)parent;
    return m_pages.count();
}

QVariant PageModel::data(const QModelIndex &index, int role) const
{
    if (index.row() < 0 || index.row() >= m_pages.count())
        return QVariant();

    const Page *page = m_pages[index.row()];

    if (role == ContentsRole)
        return page->Contents();

    return QVariant();
}

QHash<int, QByteArray> PageModel::roleNames() const {
    QHash<int, QByteArray> roles;

    roles[ContentsRole] = "contents";

    return roles;
}

