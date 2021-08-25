#include "db_manager.h"


db_manager::db_manager(const QString &db_path, const QString db_name)
{
    this->main_db = QSqlDatabase::addDatabase(DB_DRIVER, db_name);
    this->main_db.setDatabaseName(db_path);
    bool open_success = this->main_db.open();
    if (open_success){
        ok = true;
        qDebug() << OPEN_SUCCESS;
    }else{
        ok = false;
        qDebug() << OPEN_FAILURE << this->main_db.lastError().text();
    }

}

void db_manager::exec(QSqlDatabase &db,const QString &qu)
{
    db.exec(qu);
    qDebug() << db.lastError().text();
}

void db_manager::exec(const QString &q)
{
    return exec(this->main_db, q);
}

QVariantList db_manager::execQ(const QString &query, QSqlDatabase db)
{
    QSqlQuery qu(db);
    qDebug() << "Host Name : " << db.hostName();
    qu.exec(query);
    QVariantList query_result;
    while (qu.next()){
        for (auto it = 0; it <  qu.record().count(); it++){
            query_result += qu.value(it);
        }
    }
    return query_result;
}


db_manager::~db_manager()
{

}

bool db_manager::status()
{
    return this->ok;

}
