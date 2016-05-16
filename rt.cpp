#include <cassert>
#include <QtSql/QSqlDatabase>

#if defined(Q_OS_ANDROID) || defined(__APPLE__)
#include <QtCore/QDebug>
#include <QtCore/QDir>
#include <QtCore/QFile>
#include <QtCore/QFileInfo>
#include <QtCore/QStandardPaths>
#include <QtCore/QString>
#endif

#if defined(__APPLE__)
#include "TargetConditionals.h"
#endif

#include "make_unique.hpp"
#include "rt.hpp"

#if defined(Q_OS_ANDROID)
#include "android.hpp"
#endif

#include "db.hpp"
#include "dbtables.hpp"

#if defined(Q_OS_ANDROID)
#define     DB_FILE                     "assets:/resources/db/messages.db"
#define     DB_TARGET_FILE_PATH         "resources/db"
#define     DB_TARGET_FILE_NAME         "messages.db"
#elif defined(__APPLE__)
#if TARGET_IPHONE_SIMULATOR || TARGET_OS_IPHONE
#elif TARGET_OS_MAC
#define     DB_FILE                     "../Resources/resources/db/messages.db"
#define     DB_TARGET_FILE_PATH         "smsbooklet"
#define     DB_TARGET_FILE_NAME         "messages.db"
#else
#error "** Unknown Apple platform!"
#endif
#else
#define     DB_FILE                     "resources/db/messages.db"
#endif

using namespace std;
using namespace SMSDB;

std::mutex RT::m_storageMutex;
RT::Storage_ptr RT::m_storageInstance;

#if defined(Q_OS_ANDROID)
std::mutex RT::m_androidMutex;
RT::Android_ptr RT::m_androidInstance;
#endif

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

#if defined(Q_OS_ANDROID)
SMSDB::Android *RT::Android()
{
    lock_guard<mutex> lock(m_androidMutex);
    (void)lock;

    if (m_androidInstance == nullptr) {
        m_androidInstance = std::make_unique<SMSDB::Android>();
    }

    return m_androidInstance.get();
}
#endif

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

    if (m_dbInstance == nullptr) {
        m_dbInstance = std::make_unique<SMSDB::DB>(dbTargetFile.toStdString());
    }
#elif defined(__APPLE__)
#if TARGET_IPHONE_SIMULATOR || TARGET_OS_IPHONE
#elif TARGET_OS_MAC
    QString dbTargetPath = QDir::homePath() + "/" + DB_TARGET_FILE_PATH;
    QString dbTargetFile = dbTargetPath + "/" + DB_TARGET_FILE_NAME;

    if (!QFileInfo(dbTargetFile).exists()) {
        if (!QDir(dbTargetPath).exists()) {
            assert(QDir().mkpath(dbTargetPath));
        }
        assert(QFile::copy(DB_FILE, dbTargetFile));
        assert(QFile::setPermissions(dbTargetFile, QFile::ReadOwner | QFile::WriteOwner));
    }

    if (m_dbInstance == nullptr) {
        m_dbInstance = std::make_unique<SMSDB::DB>(dbTargetFile.toStdString());
    }
#else
#error "** Unknown Apple platform!"
#endif
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


