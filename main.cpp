#include <memory>
#include <QtGui/QGuiApplication>
#include "make_unique.hpp"
#include "application.hpp"
#include "dbtables.hpp"

int main(int argc, char **argv)
{
    SMSDB::DBTables::InitTables();

    std::unique_ptr<QGuiApplication> app =
            std::make_unique<QGuiApplication>(argc, argv);

    std::unique_ptr<SMSDB::Application> application =
            std::make_unique<SMSDB::Application>();
    application->Start();

    return app->exec();
}

