#include <QCoreApplication>
#include <QDebug>
#include "server.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    //MySql *msql= new MySql;

 //   msql->insertandupdate("insert into accounts(id, accname, pass, lastenter, syslvl) VALUES (2 ,'thenight61', '0931', 'no', 2);");
    Server serv(1551);
    Q_UNUSED(serv);
    return a.exec();
}
