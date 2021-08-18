#include "user.h"

user::user(QTcpSocket *socket, QObject *parent) :
    QObject(parent)
{
    m_socket = socket;
    sock_desc = socket->socketDescriptor();
}

user::user(const user &us)
{
    this->m_socket = us.m_socket;
    this->sock_desc = us.sock_desc;
}

QTcpSocket *user::socket()
{
    return this->m_socket;
}



void user::write(QByteArray msg)
{
    this->m_socket->write(msg);
}

user::~user()
{

}
