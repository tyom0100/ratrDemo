#include "user.h"

user::user(uint uid, QObject *parent) :
    QObject(parent)
{
    this->_uid = uid;
}

user::user                      (const uint uid,
                                 const QVariantList &prop_list,
                                 QObject* parent) :   user(uid)
{

    set_login(prop_list[0].toByteArray());
    set_password(prop_list[1].toByteArray());
    set_email(prop_list[2].toByteArray());
    set_username(prop_list[3].toByteArray());
    set_mobile(prop_list[4].toByteArray());
    set_rsa_public_key(prop_list[5].toByteArray());

}

void user::write                (QString msg)
{
    socket->write(msg.toLocal8Bit().constData());
    socket->flush();
}

QString user::read              ()
{
    QString temp;
    while (socket->bytesAvailable())
    {
        temp = socket->readAll();
    }
    return temp;
}

bool user::operator ==          (const user &attempt_u) const
{
    if (this->login() == attempt_u.login() &&
            this->password() == attempt_u.password()){
        return true;
    }
    return false;
}

//getters
uint user::uid                  () const
{
    return this->_uid;
}

QByteArray user::login          () const
{
    return this->_login;
}

QByteArray user::password       () const
{
    return this->_password;
}

QString user::email             () const
{
    return this->_email;
}

QString user::username          () const
{
    return this->_username;
}

QByteArray user::mobile         () const
{
    return this->_mobile;
}

QByteArray user::rsa_public_key () const
{
    return this->_rsa_public_key;
}


//setters
void user::set_login          (const QByteArray & new_login)
{
    this->_login = new_login;
}

void user::set_password       (const QByteArray &new_password)
{
    this->_password = new_password;
}

void user::set_email          (const QByteArray &new_email)
{
    this->_email = new_email;
}

void user::set_username       (const QByteArray &new_username)
{
    this->_username = new_username;
}

void user::set_mobile         (const QByteArray &new_mobile)
{
    this->_mobile = new_mobile;
}

void user::set_rsa_public_key (const QByteArray &new_rsa_public_key)
{
    this->_rsa_public_key = new_rsa_public_key;
}
