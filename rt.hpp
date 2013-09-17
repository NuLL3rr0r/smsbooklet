#ifndef RT_HPP
#define RT_HPP


#include <memory>
#include <mutex>

namespace SMSDB {
    class DB;
    class DBTables;
    class RT;
}

class SMSDB::RT
{
private:
    struct StorageStruct
    {
    };

    typedef std::unique_ptr<StorageStruct> Storage_ptr;
    typedef std::unique_ptr<SMSDB::DB> DB_ptr;
    typedef std::unique_ptr<SMSDB::DBTables> DBTables_ptr;

private:
    static std::mutex m_storageMutex;
    static Storage_ptr m_storageInstance;

    static std::mutex m_dbMutex;
    static DB_ptr m_dbInstance;

    static std::mutex m_dbTablesMutex;
    static DBTables_ptr m_dbTablesInstance;

public:
    static StorageStruct *Storage();
    static SMSDB::DB *DB();
    static SMSDB::DBTables *DBTables();
};


#endif /* RT_HPP */


