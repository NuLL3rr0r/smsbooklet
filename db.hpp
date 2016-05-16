#ifndef DB_HPP
#define DB_HPP


#include <string>

class QSqlDatabase;

namespace SMSDB {
    class DB;
}

class SMSDB::DB
{
private:
    QSqlDatabase m_db;

public:
    explicit DB(const std::string &dbFile);
    ~DB();

    void CreateTable(const std::string &table, const std::string &fields);
    void DropTable(const std::string &table);

    void Insert(const std::string &table, const std::string &fields, const int count, ...);
    void Update(const std::string &table, const std::string &where, const std::string &value,
                const std::string &set, const int count, ...);
    void Delete(const std::string &table, const std::string &where, const std::string &value);
};


#endif /* DB_HPP */


