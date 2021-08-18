#include <QTcpServer>
#include <QTcpSocket>
#include <deque>
#include <mutex>
#include <QObject>
#include <iostream>
#include <queue>
#include "user.h"
#include "manager.h"

class server : public QObject
{

public:
    server();
    ~server() = default;

signals:

public slots:

    void new_connection();

private:
    QTcpServer *m_server;
    manager *m_manager;

};
