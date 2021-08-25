#include "server.h"

server::server(QObject *parent) :
    QObject(parent)
{
    uid = 0;
    m_server = new QTcpServer(this);
    master = new manager();
    connect(m_server,SIGNAL(newConnection()),this,SLOT(new_connection()));
}

server::~server()
{
    //frees
    for (auto &client : online_clients){
        client->deleteLater();
    }
    for (auto &key :  clients_list){
        key->deleteLater();
    }

    delete this->master;
    delete this->m_server;
}

void server::new_connection()
{
    QMutex mutex;
    mutex.lock();
    online_clients.append(m_server->nextPendingConnection());
    connect(online_clients.back(), &QTcpSocket::readyRead, this, &server::read);
    connect(online_clients.back(), &QTcpSocket::disconnected, this, &server::closed_connection);
    mutex.unlock();
}

void server::closed_connection()
{

}

void server::run_server(QHostAddress address, int port)
{
    if(!m_server->listen(address,port)){
        qDebug() << "Server could not start!";
    }else{
        qDebug() << "Server started";
    }
}

void server::read()
{
    QTcpSocket* temp = static_cast<QTcpSocket*>(sender());
    QByteArray buf = temp->readAll();
    std::string str = static_cast<std::string>(buf);
    str.erase(std::remove(str.begin(), str.end(), '\r'), str.end());
    str.erase(std::remove(str.begin(), str.end(), '\n'), str.end());
    buf = QByteArray::fromStdString(str);
    auto em_res = master->process_emit(buf);
    distribute_emit_result(em_res, buf);

}

void server::distribute_emit_result(const short &em_res,const QByteArray &buf)
{
    //create key_value construction for sign upped user

    auto map_for_sign_up = [buf](){
        QXmlStreamReader reader;
        QList<QPair<QByteArray, QByteArray>> keymap;
        qDebug() << "Caught buffer : "<< buf;
        reader.addData(buf);
        while(!reader.atEnd() && !reader.hasError()) {
            if (reader.readNext() == QXmlStreamReader::StartElement) {
                for (auto key : SIGN_UP_KEYS){ //SEARCH IN SIGNUP KEYS
                   if (reader.name().toString() == QString(key) && reader.name().toString().length() > 0){
                       keymap.append(QPair<QByteArray, QByteArray>(key,
                                                                   QByteArray::fromStdString(reader.readElementText().toStdString())));
                   }
                }
            }
        }

        return keymap;
    };
    auto map_for_log_in = [buf](){
        QXmlStreamReader reader;
        QList<QPair<QByteArray, QByteArray>> keymap;
        qDebug() << "Caught buffer : "<< buf;
        reader.addData(buf);
        while(!reader.atEnd() && !reader.hasError()) {
            if (reader.readNext() == QXmlStreamReader::StartElement) {
                for (auto key : LOG_IN_KEYS){  //SEARCH IN LOGIN KEYS
                   if (reader.name().toString() == QString(key) && reader.name().toString().length() > 0){
                       keymap.append(QPair<QByteArray, QByteArray>(key,
                                      QByteArray::fromStdString(reader.readElementText().toStdString())));

                   }
                }
            }
        }
        qDebug() << keymap;
        return keymap;
    };

        switch (em_res) {
            case commands::SIGN_UP:
               master->sign_up_process(map_for_sign_up(),
                                       clients_list     ,
                                       this->uid)       ;
                    break;
            case commands::LOG_IN:
            qDebug() << "Processing";
                master->log_in_process(map_for_log_in(),clients_list);
                    break;
            case commands::STAFFED_REQUEST:
               // master->stuffed_request_process();
                    break;
            case commands::ILLEGAL_REQUEST:
               // master->illegal_request_process();
                    break;
        default:
            master->illegal_request_process();
            break;
        }

}



