#include "manager.h"

manager::manager()
{
    db_master = new db_manager(DEFAULT_MAINPATH+QString("main.db"));
}

manager::~manager()
{
    delete this->db_master;
}

QString manager::generate_user_db_path(uint uid, const QString mainpath)
{
    QString path = "'"+ mainpath + QString::number(uid) + ".db'";
    return path;
}
                                                              //field  //type
void manager::create_table(const QString &table_name,const QList<QPair<QByteArray, QByteArray>> &properties, QSqlDatabase db)
{
    //here bugs

    QSqlQuery q(db);
    QString query = "CREATE TABLE \""+table_name+"\" (";
    for (auto it = properties.begin(), end = properties.end(); it != end; it++) {
        query += "\"" + it->first + "\" TEXT ,";
    }
    query.truncate(query.lastIndexOf(QChar(',')));
    query+=");";
    q.exec(query);
}

void manager::drop_table(const QString &table_name, QSqlDatabase &db)
{
       QSqlQuery to_drop_qu(db);
       to_drop_qu.exec("DROP TABLE "+table_name+";");
}

bool manager::check_emmited_content(const QList<QPair<QByteArray, QByteArray>> &bufmap,
                                    const QVector<QByteArray> &keys)
{
    bool ok;
    QByteArray keybyte;
    for (const auto &key : keys){
        keybyte = QByteArray::fromStdString(key.toStdString());
        ok = false;
        for (const auto &pair : bufmap){
            if (keybyte == pair.first){
                ok = true;
            }
        }
        if (!ok){
            return false;
        }
    }
    return true;
}

void manager::add_user_to_db(const user &u, QSqlDatabase db)
{
    QString query(QString("INSERT INTO ") +USERS_TABLE_NAME+" VALUES ("+QString::number(u.uid())+", "+generate_user_db_path(u.uid())+");");
    QSqlQuery q(db);
    db_master->exec(query);
}


void manager::add_big_user_to_own_db(const QList<QPair<QByteArray, QByteArray>> &props, QSqlDatabase db)
{
    QString query(QString("INSERT INTO ") +USERS_OWN_DB_LOGIN_DATA_TABLE_NAME+" VALUES (");
    for (auto &it : props){
        query+= "\""+it.second + "\",";
    }
    query = query.left(query.lastIndexOf(',')); // remove last ','
    query += ");";
    QSqlQuery q(db);
    q.exec(query);
    return;
}

user manager::get_big_user_from_db(const uint uid,const QString &path)
{
    db_manager ret_master(path,"RetrieveMaster");
    QVariantList ret_data = ret_master.execQ(QString("SELECT * FROM ")+USERS_OWN_DB_LOGIN_DATA_TABLE_NAME, ret_master.get_db());
    return user(uid, ret_data);
}

/*--------------------------------------------------------------------------------------------------*
 *                                                ___                                               *
 *                                               / x \                                              *
 *                                               \_X_/                                              *
 *                                                 |                                                *
 *                                         _________________                                        *
 *                                        |   emit_process  |                                       *
 *                                         -----------------                                        *
 *                                                0123                                              *
 *                                                 |                                                *
 *                                       ______________________                                     *
 *                        |-------------|distribute_emit_result|--------|                           *
 *                        |              ----------------------         |                           *
 *             0----------01---------1                   2--------------23-------------3            *
 *             |                     |                   |                             |            *
 *     _______________   ______________   _______________________    _______________________        *
 *    |sign_up_process| |log_in_process| |stuffed_request_process|  |illegal_request_process|       *
 *     ---------------   --------------   -----------------------    -----------------------        *
 *                                                                                                  *
 *                                                                                                  *
 ---------------------------------------------------------------------------------------------------*/

bool manager::is_staffed_request(QByteArray &buf)
{
    char* buf_ch = buf.data();
    unsigned int len = buf.length();
    QXmlStreamReader xmlReader;
    xmlReader.addData(buf_ch);
    if (xmlReader.hasError() || !len){
            return false;
    }

    return true;
}


short manager::process_emit(QByteArray &buffer)
{
    QXmlStreamReader xmlReader(buffer);
    if (!is_staffed_request(buffer)){
        return commands::ILLEGAL_REQUEST; // if request form cannot be processed
    }//else
    while (xmlReader.readNextStartElement()){
            if (QString(xmlReader.name().toString()) == QString(SIGN_UP_STRING)){
                return commands::SIGN_UP;
            }else if (QString(xmlReader.name().toString()) == QString(LOG_IN_STRING)){
                return commands::LOG_IN;
            }
    }

    return commands::STAFFED_REQUEST;     //if request is legal but is not login/signup request , then is considered to be staffed (ordinary request)
}
                                                // key       // value
void manager::sign_up_process(const QList<QPair<QByteArray, QByteArray>> bufmap,
                                  QHash<QTcpSocket*, user*> &c_l,
                                      uint &uid)
{
    if (!check_emmited_content(bufmap,SIGN_UP_KEYS)){
        qDebug() << "Damaged Data";
        return;
    }
    user *new_client  = new user(uid++);
    new_client->set_login(bufmap[0].second);
    new_client->set_password(bufmap[1].second);
    new_client->set_email(bufmap[2].second);
    new_client->set_username(bufmap[3].second);
    new_client->set_mobile(bufmap[4].second);
    new_client->set_rsa_public_key(bufmap[5].second);

    add_user_to_db(*new_client, get_db_manager()->get_db()); // add already created user to database
    QString path = generate_user_db_path(new_client->uid());
    std::string path_s = path.toStdString();
    path_s.erase(std::remove(path_s.begin(), path_s.end(), '\''), path_s.end());
    path = QString::fromStdString(path_s);
    qDebug() << path;
    db_manager signup_master(path, "LoginMaster");
    if (signup_master.status() == false){
       qDebug() <<"Failed to open database";
    }
    create_table(USERS_OWN_DB_LOGIN_DATA_TABLE_NAME, bufmap, signup_master.get_db());
    add_big_user_to_own_db(bufmap,signup_master.get_db());
    delete new_client;
    return;

}

bool manager::log_in_process(const QList<QPair<QByteArray, QByteArray>> &bufmap,
                                QHash<QTcpSocket*, user*> &c_l)
{
    if (!check_emmited_content(bufmap,LOG_IN_KEYS)){
        qDebug() << "Damaged Data";
        return false;
    }
    QByteArray uid (bufmap[0].second);
    qDebug() << uid;
    QString path;
    QVariantList q_l=  db_master->execQ(QString("SELECT path FROM ")+USERS_TABLE_NAME+" WHERE uid="+uid, db_master->get_db());
    if (q_l.size() == 0){
        return 0;//DB didn't responded and has not existing user
    }
    path = q_l[0].toString();

    qDebug() << " Retrieved path : " <<  path;
    get_big_user_from_db(uid.toUInt(),path);

}

void manager::stuffed_request_process(const QList<QPair<QByteArray, QByteArray>> &bufmap,
                                            user &client)
{
    qDebug() << "StuffedPROC";
}

void manager::illegal_request_process(void)
{
    qDebug() << "IllegalPROC";
}

void manager::remove_user_from_db(const long long &uid, QSqlDatabase db)
{
    QString query (QString("DELETE FROM ")+ USERS_TABLE_NAME+ " WHERE uid="+QString::number(uid));
    QSqlQuery qu(db);
    qu.exec(query);
    if(qu.lastError() != QString(QUERY_SUCCESS)){
        qDebug() << "Failed to execute query request";
    }return;
}


