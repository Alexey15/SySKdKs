#include "mysql.h"
#include <QSqlError>
#include <QSqlQuery>
#include <QList>
#include <QSqlRecord>
#include <QSqlField>
#include <QSqlResult>
#include <QDebug>

MySql::MySql(QObject *parent) : QObject(parent)
{
    FileSettings *stng = new FileSettings("MySql");
    if(stng->isNotFile()){
        qDebug() << "Can't open file settings.txt, please contact the administrator for assistance\n";
        haserr=true;
        return;
    } else{
    mDataBase = QSqlDatabase::addDatabase("QMYSQL");
    mDataBase.setHostName(stng->getProperty("hostname"));
    mDataBase.setDatabaseName(stng->getProperty("databasename"));
    mDataBase.setUserName(stng->getProperty("login"));
    mDataBase.setPassword(stng->getProperty("password"));
    mDataBase.setPort(stng->getProperty("port").toInt());
    mDataBase.setConnectOptions();
    if(!mDataBase.open()){
        qDebug() << "Can't connect to DB\n";
        haserr=true;
        return;
    }
    }
}

bool MySql::isError(){
    return haserr;
}

void MySql::insertandupdate(QString zapros){
    QSqlQuery q;
    q.exec(zapros);
    q.isActive();
    mDataBase.commit();
}

int MySql::getQueryCount(QString tname){
    QSqlQuery q;
    q.exec("select * from "+tname);
    q.isActive();
    int res=0;
    while (q.next()){
        res++;
    }
    return res;
}
int MySql::getQueryCountIf(QString tname, QString If){
    QSqlQuery q;
    q.exec("select * from "+tname+" where "+If+";");
    q.isActive();
    int res=0;
    while (q.next()){
        res++;
    }
    return res;
}

QSqlQuery MySql::select(QString zapros){
    QSqlQuery q;
    q.exec(zapros);
    q.isActive();
    return q;
}

void MySql::discon(){
    mDataBase.close();
}
