#ifndef USER_H
#define USER_H

#include <QObject>
#include <QTcpSocket>
#include <QDebug>

class user : public QObject
{
    Q_OBJECT
public:
    explicit user(QObject *parent = 0);

    QTcpSocket *socket;

    void write(QString msg);
    QString read();

signals:

public slots:

private:
    QString temp;

};

#endif // USER_H
