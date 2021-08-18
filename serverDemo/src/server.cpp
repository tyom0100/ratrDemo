#include "server.h"

server::server(QObject *parent) :
    QObject(parent)
{
    m_server = new QTcpServer(this);
    connect(m_server,SIGNAL(newConnection()),this,SLOT(new_connection()));
}

void server::new_connection()
{
    user *client = new user(this);
    client->socket = m_server->nextPendingConnection();
    connect(client->socket,SIGNAL(disconnected()),this,SLOT(closed_connection()));
    client->write("Connected!\n");
    qDebug() << client->socket << "Connected!";
    mutex.lock();
    clients_list.insert((client->socket), client);
    mutex.unlock();
    connect(client->socket,SIGNAL(readyRead()),this,SLOT(read_waiting_socket()));
}

void server::closed_connection()
{
    qDebug() << sender() << "Closed connection!";
    mutex.lock();
    clients_list.remove((QTcpSocket*)sender());
    mutex.unlock();
    sender()->deleteLater();
}


void server::run_server(QHostAddress address, int port)
{
    if(!m_server->listen(address,port))
    {
        qDebug() << "Server could not start!";
    }
    else
    {
        qDebug() << "Server started";
    }
}

void server::read_waiting_socket()
{
    mutex.lock();
    qDebug() << clients_list[(QTcpSocket*)sender()]->read();
    mutex.unlock();
}
