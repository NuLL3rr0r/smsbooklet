#include <QtCore/QDebug>
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlError>
#include <QtSql/QSqlQuery>
#include <cassert>
#include <cstdarg>
#include "db.hpp"

using namespace std;
using namespace SMSDB;

DB::DB(const string &dbFile)
{
    m_db = QSqlDatabase::addDatabase("QSQLITE");
    m_db.setDatabaseName(dbFile.c_str());

    if (!m_db.open()) {
        qDebug() << m_db.lastError();
        assert(false);
    }
}

DB::~DB()
{
}

void DB::CreateTable(const std::string &table, const std::string &fields)
{
    QSqlQuery query(("CREATE TABLE IF NOT EXISTS [" + table + "] ("
                    + fields
                     + ");").c_str());
    query.exec();
}

void DB::DropTable(const std::string &table)
{
    QSqlQuery query(("DROP TABLE IF EXISTS [" + table + "];").c_str());
    query.exec();
}

void DB::Insert(const std::string &table, const std::string &fields, const int count, ...)
{
    va_list args;
    va_start(args, count);

    string ph;
    for (int i = 0; i < count; ++i) {
        if (i != 0)
            ph += ", ";
        ph += "?";
    }


    QSqlQuery query(("INSERT OR IGNORE INTO [" + table + "] "
                    "(" + fields + ") "
                    "VALUES (" + ph + ");").c_str());

    for(int i = 0; i < count; ++i) {
         query.addBindValue(va_arg(args, char*));
    }

    va_end(args);

    query.exec();
}

void DB::Update(const std::string &table, const std::string &where, const std::string &value,
                const std::string &set, const int count, ...)
{
    va_list args;
    va_start(args, count);

    QSqlQuery query(("UPDATE [" + table + "] "
                     "SET " + set + " "
                     "WHERE " + where + "=?;").c_str());

    for(int i = 0; i < count; ++i) {
         query.addBindValue(va_arg(args, char*));
    }

    va_end(args);

    //query.bindValue((":" + where).c_str(), value.c_str());
    query.addBindValue(value.c_str());

    query.exec();
}

void DB::Delete(const std::string &table, const std::string &where, const std::string &value)
{
    QSqlQuery query(("DELETE FROM [" + table + "] "
                     "WHERE " + where + "=?;").c_str());
    //query.bindValue((":" + where).c_str(), value.c_str());
    query.addBindValue(value.c_str());
    query.exec();
}

