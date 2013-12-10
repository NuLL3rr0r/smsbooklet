#include <QtSql/QSqlDatabase>
#include "dbtables.hpp"
#include "db.hpp"
#include "rt.hpp"

using namespace std;
using namespace SMSDB;

void DBTables::InitTables()
{
    RT::DB()->CreateTable(RT::DBTables()->Table(TableName::Categories),
                          RT::DBTables()->Fields(TableName::Categories));
    RT::DB()->CreateTable(RT::DBTables()->Table(TableName::SubCategories),
                          RT::DBTables()->Fields(TableName::SubCategories));
    RT::DB()->CreateTable(RT::DBTables()->Table(TableName::Messages),
                          RT::DBTables()->Fields(TableName::Messages));
}

DBTables::DBTables()
{
    InitHashes();
}

string DBTables::Table(TableName id)
{
    if (m_tablesHash.find(id) != m_tablesHash.end()) {
        return m_tablesHash[id];
    } else {
        return "{?}";
    }
}

string DBTables::Fields(TableName id)
{
    if (m_fieldsHash.find(id) != m_fieldsHash.end()) {
        return m_fieldsHash[id];
    } else {
        return "{?}";
    }
}

void DBTables::InitHashes()
{
    m_tablesHash[TableName::Categories] = "categories";
    m_tablesHash[TableName::SubCategories] = "subcategories";
    m_tablesHash[TableName::Messages] = "messages";

    m_fieldsHash[TableName::Categories] =
            " id TEXT NOT NULL, "
            " name TEXT NOT NULL, "
            " icon TEXT NOT NULL, "
            " PRIMARY KEY ( id, name ASC ) ";

    m_fieldsHash[TableName::SubCategories] =
            " id TEXT NOT NULL, "
            " name TEXT NOT NULL, "
            " catid TEXT NOT NULL, "
            " icon TEXT NOT NULL, "
            " lastpage TEXT, "
            " PRIMARY KEY ( id, name, catid ASC ) ";

    m_fieldsHash[TableName::Messages] =
            " id TEXT NOT NULL, "
            " subcatid TEXT NOT NULL, "
            " text TEXT NOT NULL, "
            " fav INTEGER NOT NULL, "
            " PRIMARY KEY ( id, subcatid ASC ) ";
}


