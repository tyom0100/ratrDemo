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
#include "user.h"

class server : public QObject
{
    Q_OBJECT
public:
    explicit server(QObject *parent = 0);
    void run_server(QHostAddress address, int port);

signals:

public slots:
    void new_connection();
    void closed_connection();
    void read_waiting_socket();

private:
    QTcpServer *m_server;
    QHash<QTcpSocket*, user*> clients_list;
    QHash<QTcpSocket*, user*>::iterator i;
    QMutex mutex;
};

#endif // SERVER_H
