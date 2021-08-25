#ifndef SERVER_H
#define SERVER_H

#include <QObject>
#include <QDebug>
#include <QTcpServer>
#include <QTcpSocket>
#include <QHash>
#include <QHashIterator>
#include <QString>
#include <QMutex>
#include "manager.h"

class server : public QObject
{
    Q_OBJECT
public:
    explicit server(QObject *parent = 0);
    ~server();
    void run_server(QHostAddress address, int port);

signals:

public slots:
    void new_connection();
    void closed_connection();
    void read();

protected:
    void distribute_emit_result(const short&,const QByteArray &);

private:

    uint uid;

    manager *master;
    QTcpServer *m_server;
    QList <QTcpSocket*> online_clients;
    QHash<QTcpSocket*, user*> clients_list;
    QHash<QTcpSocket*, user*>::iterator i;

};

#endif // SERVER_H

