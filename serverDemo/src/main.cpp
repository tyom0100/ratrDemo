#include <server.h>
#include <QCoreApplication>

int main(int argc, char *argv[])
{
    QCoreApplication a (argc, argv);

    server s;
    return a.exec();
}
