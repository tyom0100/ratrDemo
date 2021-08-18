#include "user.h"

user::user(QObject *parent) :
    QObject(parent)
{
}

void user::write(QString msg)
{
    socket->write(msg.toLocal8Bit().constData());
    socket->flush();
}

QString user::read()
{
    while (socket->bytesAvailable())
    {
        temp = socket->readAll();
    }
    return temp;
}
