#ifndef MANAGER_H
#define MANAGER_H

#include <QObject>
#include <QXmlStreamReader>
#include <QXmlStreamAttribute>

#include "db_manager.h"
#include "user.h"



class manager
{
public:

    explicit manager                ();
    ~manager                        ();
    QString generate_user_db_path   (uint uid, const QString mainpath = DEFAULT_MAINPATH);
    db_manager* get_db_manager      ()const {return this->db_master;}
    user get_big_user_from_db       (const uint uid,const QString&); // own_db = like 2.db 3.db , where stores user expanded data
    void create_table               (const QString& ,const QList<QPair<QByteArray, QByteArray>> &properties, QSqlDatabase db);
    void add_user_to_db             (const user &, QSqlDatabase db);
    void remove_user_from_db        (const long long&, QSqlDatabase db);
    void add_big_user_to_own_db     (const QList<QPair<QByteArray, QByteArray>> &props, QSqlDatabase db);
    void sign_up_process            (const QList<QPair<QByteArray, QByteArray>> bufmap, QHash<QTcpSocket*, user*> &c_l, uint &uid);
    void stuffed_request_process    (const QList<QPair<QByteArray, QByteArray>> &bufmap, user &client);
    void illegal_request_process    (void);
    void drop_table                 (const QString &table_name, QSqlDatabase &db);
    short process_emit              (QByteArray &);
    bool log_in_process             (const QList<QPair<QByteArray, QByteArray>> &bufmap, QHash<QTcpSocket*, user*> &c_l);
    bool is_staffed_request         (QByteArray&);
    bool check_emmited_content      (const QList<QPair<QByteArray,QByteArray>> &, const QVector<QByteArray> &);

private:
    db_manager* db_master;

};

#endif // MANAGER_H
