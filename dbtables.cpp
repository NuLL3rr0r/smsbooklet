#include <QtSql/QSqlDatabase>
#include "dbtables.hpp"
#include "db.hpp"
#include "rt.hpp"

using namespace std;
using namespace SMSDB;

void DBTables::InitTables()
{
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
    m_tablesHash[TableName::Messages] = "messages";
    m_fieldsHash[TableName::Messages] = " category TEXT NOT NULL, "
            " message TEXT NOT NULL, "
            " PRIMARY KEY ( category, message ASC ) ";
}

