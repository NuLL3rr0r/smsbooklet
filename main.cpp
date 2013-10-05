#include <memory>
#include <cstdlib>
#include <ctime>
#include <QtGui/QGuiApplication>
#include "make_unique.hpp"
#include "application.hpp"
#include "dbtables.hpp"

int main(int argc, char **argv)
{
    srand(time(NULL));

    SMSDB::DBTables::InitTables();

    std::unique_ptr<QGuiApplication> app =
            std::make_unique<QGuiApplication>(argc, argv);

    std::unique_ptr<SMSDB::Application> application =
            std::make_unique<SMSDB::Application>();
    application->Start();

    return app->exec();
}

