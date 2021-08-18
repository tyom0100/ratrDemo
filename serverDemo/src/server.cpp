#include "server.h"

server::server()
{
    m_server = new QTcpServer(this);
    m_manager = new manager();
    connect(m_server, &QTcpServer::newConnection, this, &server::new_connection);

    if (!m_server->listen(QHostAddress::Any, 33333)){
        qDebug() << "Server could not start :";
    }else{
        qDebug() << "Server started :";
    }
}


void server::new_connection()
{

}

