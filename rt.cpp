#include <QtSql/QSqlDatabase>
#include "make_unique.hpp"
#include "rt.hpp"
#include "db.hpp"
#include "dbtables.hpp"

#define     DB_FILE            "resources/db/messages.db"

using namespace std;
using namespace SMSDB;

std::mutex RT::m_storageMutex;
RT::Storage_ptr RT::m_storageInstance;

std::mutex RT::m_dbMutex;
RT::DB_ptr RT::m_dbInstance;

std::mutex RT::m_dbTablesMutex;
RT::DBTables_ptr RT::m_dbTablesInstance;

RT::StorageStruct *RT::Storage()
{
    lock_guard<mutex> lock(m_storageMutex);
    (void)lock;

    if (m_storageInstance == nullptr) {
        m_storageInstance = std::make_unique<RT::StorageStruct>();
    }

    return m_storageInstance.get();
}

SMSDB::DB *RT::DB()
{
    lock_guard<mutex> lock(m_dbMutex);
    (void)lock;

    if (m_dbInstance == nullptr) {
        m_dbInstance = std::make_unique<SMSDB::DB>(DB_FILE);
    }

    return m_dbInstance.get();
}

SMSDB::DBTables *RT::DBTables()
{
    lock_guard<mutex> lock(m_dbTablesMutex);
    (void)lock;

    if (m_dbTablesInstance == nullptr) {
        m_dbTablesInstance = std::make_unique<SMSDB::DBTables>();
    }

    return m_dbTablesInstance.get();
}



