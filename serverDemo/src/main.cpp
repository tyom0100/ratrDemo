#include <QCoreApplication>
#include "server.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    server Server;
    Server.run_server(QHostAddress::Any, 1234);

    return a.exec();
}
