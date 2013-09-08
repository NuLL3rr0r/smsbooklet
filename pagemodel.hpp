#ifndef PAGEMODEL_HPP
#define PAGEMODEL_HPP

#include <QAbstractListModel>
#include <QStringList>

namespace SMSDB {
    class Page;
    class PageModel;
}

class SMSDB::Page
{
private:
    QString m_contents;

public:
    explicit Page(const QString &contents);
    virtual ~Page();

public:
    QString Contents() const;
};

class SMSDB::PageModel : public QAbstractListModel
{
    Q_OBJECT

    Q_PROPERTY( int count READ rowCount NOTIFY countChanged )

public:
    enum PageRoles {
        ContentsRole = Qt::UserRole + 1
    };

private:
    QList<Page*> m_pages;

public:
    explicit PageModel(QObject *parent = 0);
    virtual ~PageModel();

signals:
    void countChanged(int);

public:
    Q_INVOKABLE void clear();
    Q_INVOKABLE QVariantMap get(int index);
    Q_INVOKABLE void remove(int index);

public:
    void AddPage(Page *page);

public:
    int rowCount(const QModelIndex &parent = QModelIndex()) const;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;

protected:
    QHash<int, QByteArray> roleNames() const;
};

#endif // PAGEMODEL_HPP

