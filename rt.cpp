#include <cassert>
#include <QtSql/QSqlDatabase>

#if defined(Q_OS_ANDROID)
#include <QtCore/QDebug>
#include <QtCore/QDir>
#include <QtCore/QFile>
#include <QtCore/QFileInfo>
#include <QtCore/QStandardPaths>
#include <QtCore/QString>
#endif

#include "make_unique.hpp"
#include "rt.hpp"
#include "db.hpp"
#include "dbtables.hpp"

#if defined(Q_OS_ANDROID)
#define     DB_FILE                     "assets:/resources/db/messages.db"
#define     DB_TARGET_FILE_PATH         "resources/db"
#define     DB_TARGET_FILE_NAME         "messages.db"
#else
#define     DB_FILE                     "resources/db/messages.db"
#endif

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

#ifdef Q_OS_ANDROID
    QString dbTargetPath = QDir::currentPath() + "/" + DB_TARGET_FILE_PATH;
    QString dbTargetFile = dbTargetPath + "/" + DB_TARGET_FILE_NAME;

    if (!QFileInfo(dbTargetFile).exists()) {
        if (!QDir(dbTargetPath).exists()) {
            assert(QDir().mkpath(dbTargetPath));
        }
        assert(QFile::copy(DB_FILE, dbTargetFile));
        assert(QFile::setPermissions(dbTargetFile, QFile::ReadOwner | QFile::WriteOwner));
    }

    /*
    QString dbTargetPath = QStandardPaths::writableLocation(QStandardPaths::DataLocation);
    assert(dbTargetPath != "");
    QString dbTargetFile = dbTargetPath + "/" + DB_TARGET_FILE_NAME;
    if (!QFileInfo(dbTargetFile).exists()) {
        qDebug() << dbTargetFile;
        assert(QFile::copy(DB_FILE, dbTargetFile));
        assert(QFile::setPermissions(dbTargetFile, QFile::ReadOwner | QFile::WriteOwner));
    }
    */

    if (m_dbInstance == nullptr) {
        m_dbInstance = std::make_unique<SMSDB::DB>(dbTargetFile.toStdString());
    }
#else
    if (m_dbInstance == nullptr) {
        m_dbInstance = std::make_unique<SMSDB::DB>(DB_FILE);
    }
#endif

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



