#ifndef MYSQL_H
#define MYSQL_H

#include <QObject>
#include <QtSql/QSqlDatabase>
#include <filesettings.h>

class MySql : public QObject
{
    Q_OBJECT
public:
    explicit MySql(QObject *parent = nullptr);
    void insertandupdate(QString zapros);
    QSqlQuery select(QString zapros);
    int getQueryCount(QString tname);
    int getQueryCountIf(QString tname, QString If);
    void discon();
    bool isError();
signals:

public slots:

private:
    QSqlDatabase mDataBase;
    bool haserr=false;
};

#endif // MYSQL_H
