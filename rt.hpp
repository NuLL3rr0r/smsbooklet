#ifndef RT_HPP
#define RT_HPP


#include <memory>
#include <mutex>

namespace SMSDB {
#if defined(Q_OS_ANDROID)
    class Android;
#endif
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
#if defined(Q_OS_ANDROID)
    typedef std::unique_ptr<SMSDB::Android> Android_ptr;
#endif
    typedef std::unique_ptr<SMSDB::DB> DB_ptr;
    typedef std::unique_ptr<SMSDB::DBTables> DBTables_ptr;

private:
    static std::mutex m_storageMutex;
    static Storage_ptr m_storageInstance;

#if defined(Q_OS_ANDROID)
    static std::mutex m_androidMutex;
    static Android_ptr m_androidInstance;
#endif

    static std::mutex m_dbMutex;
    static DB_ptr m_dbInstance;

    static std::mutex m_dbTablesMutex;
    static DBTables_ptr m_dbTablesInstance;

public:
    static StorageStruct *Storage();
#if defined(Q_OS_ANDROID)
    static SMSDB::Android *Android();
#endif
    static SMSDB::DB *DB();
    static SMSDB::DBTables *DBTables();
};


#endif /* RT_HPP */


