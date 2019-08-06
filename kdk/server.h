#ifndef SERVER_H
#define SERVER_H

#include <QTcpServer>
#include "mysql.h"
#include <QTcpSocket>
#include <QByteArray>
#include <QDebug>
#include <QUdpSocket>
#include <QDate>
#include <QSqlRecord>
#include <QDataStream>
#include <QSqlQuery>
#include <QObject>
#include <QMap>
#include <QList>
#include <QTimer>
extern QMap<int ,QTcpSocket*> uid;
extern QMap<QTcpSocket*, QString> ocopms;
extern QList<QTcpSocket*> adminlst;
extern MySql msql;

class Server : public QObject
{
    Q_OBJECT
private:
    QTcpServer* m_ptcpServer;
    quint16     m_nNextBlockSize;
    QTimer *timer;
    void sendToClient(QTcpSocket* pSocket, const QString& str);
    void wakeOnLan(QString MAC);
    int randomBetween(int low, int high, int seed);

public:
    explicit Server(quint16 nPort, QObject *parent = nullptr);
signals:

private slots:
    virtual void slotNewConnection();
            void slotReadClient();
            void slotClientDisconnect();
            void procesOneThing();
};

#endif // SERVER_H
