#ifndef DB_MANAGER_H
#define DB_MANAGER_H

#include <QObject>
#include <QtSql/QSqlDatabase>
#include <QDebug>
#include <QSqlError>
#include <QSqlQuery>
#include <QSqlRecord>
#include "constants.h"

constexpr auto DB_DRIVER = "QSQLITE";
constexpr auto DB_DEFAULT_NAME = "Main Database";
constexpr auto OPEN_SUCCESS = "Database opened successfully";
constexpr auto OPEN_FAILURE = "Failed to connect with database";
constexpr auto QUERY_SUCCESS = "Executed properly";

class db_manager
{
public:
    db_manager          () = delete;
    explicit db_manager (const QString &db_path, const QString db_name = DB_DEFAULT_NAME);
    QSqlDatabase get_db ()const{return this->main_db;};
    static void exec    (QSqlDatabase &, const QString &);
    void exec           (const QString&);
    QVariantList execQ  (const QString&, QSqlDatabase db);
    ~db_manager         ();
    bool status         ();

private:
    QSqlDatabase main_db;
    bool ok;
};

#endif // DB_MANAGER_H
