#ifndef DBTABLES_HPP
#define DBTABLES_HPP


#include <string>
#include <unordered_map>

namespace SMSDB {
    class DBTables;
}

class SMSDB::DBTables
{
public:
    enum class TableName : unsigned char {
        Messages
    };

private:
    template <typename _T>
    struct Hasher
    {
        std::size_t operator()(const _T &t) const
        {
            return std::hash<unsigned char>()(static_cast<unsigned char>(t));
        }
    };

private:
    std::unordered_map<TableName, std::string, Hasher<TableName>> m_tablesHash;
    std::unordered_map<TableName, std::string, Hasher<TableName>> m_fieldsHash;

public:
    static void InitTables();

public:
    explicit DBTables();

    std::string Table(TableName id);
    std::string Fields(TableName id);

private:
    void InitHashes();
};


#endif /* DBTABLES_HPP */


