#ifndef USER_H
#define USER_H

#include <QObject>
#include <QString>
#include <QTcpSocket>

class user : public QObject
{
Q_OBJECT
public:
    user(QTcpSocket *socket, QObject *parent = nullptr);
    user(const user &us);
    QTcpSocket *socket();
    void write(QByteArray msg);
    ~user();

private:
    QTcpSocket *m_socket;
    qint16 sock_desc;

};

#endif // USER_H
