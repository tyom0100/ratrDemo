#ifndef CONSTANTS_H
#define CONSTANTS_H

#define USERS_TABLE_NAME "users"
#define DEFAULT_MAINPATH "/home/aram/ratrDemo/serverDemo/DB/"
#define USERS_OWN_DB_LOGIN_DATA_TABLE_NAME "user_login_data"
#include <QByteArray>
#include <QVector>

constexpr auto SIGN_UP_STRING = "sign_up";
constexpr auto LOG_IN_STRING = "log_in";



enum commands{
    SIGN_UP,            //if emmited socket buffer is requesting new client creation
    LOG_IN,             //if emmited socket buffer is requesting to allow access to already created client
    STAFFED_REQUEST,    //if emmited socket buffer has already been accessed to proper client data, and requesting in legal form
    ILLEGAL_REQUEST     //if emmited socket buffer cannot be processed and buffer form is illegal
};
static QVector<QByteArray> SIGN_UP_KEYS {
                                         "login",
                                         "password",
                                         "email",
                                         "username",
                                         "mobile",
                                         "public_rsa_key"
                                         };

static QVector<QByteArray> LOG_IN_KEYS  {
                                        "uid",
                                        "login_attempt",
                                        "pswd_attempt"
                                        };

#endif // CONSTANTS_H
//<log_in> <uid> 0 </uid> <login_attempt> 1234 </login_attempt> <pswd_attempt> 111 </pswd_attempt> </log_in>
