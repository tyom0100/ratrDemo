#ifndef USER_H
#define USER_H

#include <QObject>
#include <QTcpSocket>
#include <QDebug>

class user : public QObject
{
    Q_OBJECT
public:
    explicit user             (uint uid, QObject *parent = 0);
    explicit user             (uint uid, const QVariantList &prop_list, QObject *parent = 0);
    QTcpSocket *socket;
    void write                (QString msg);
    QString read              ();
    bool operator==           (const user &attempt_u) const;

    uint uid                  () const;
    QByteArray login          () const;
    QByteArray password       () const;
    QString email             () const;
    QString username          () const;
    QByteArray mobile         () const;
    QByteArray rsa_public_key () const;

    void set_login            (const QByteArray&);
    void set_password         (const QByteArray&);
    void set_email            (const QByteArray&);
    void set_username         (const QByteArray&);
    void set_mobile           (const QByteArray&);
    void set_rsa_public_key   (const QByteArray&);

public slots:

private:
    uint _uid;
    QByteArray _login;
    QByteArray _password;
    QByteArray _email;
    QByteArray _username;
    QByteArray _mobile;
    QByteArray _rsa_public_key;

};

#endif // USER_H
