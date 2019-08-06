#include "server.h"
#include <QTime>

QMap<int ,QTcpSocket*> uid;
QMap<QTcpSocket*, QString> ocopms;
QList<QTcpSocket*> adminlst;
MySql msql;
Server::Server(quint16 nPort, QObject *parent) : QObject(parent), m_nNextBlockSize(0)
{
    if(!msql.isError()){
    m_ptcpServer = new QTcpServer(this);
        if (!m_ptcpServer->listen(QHostAddress::Any, nPort)) {
            qDebug()<<"Unable to start the server:" + m_ptcpServer->errorString()<<"\n";
            m_ptcpServer->close();
            return;
        }else {
            qDebug()<<"Server has succesful started!\n";
        }
        connect(m_ptcpServer, SIGNAL(newConnection()),this,SLOT(slotNewConnection()));
        procesOneThing();
        timer = new QTimer(this);
        connect(timer, SIGNAL(timeout()), this, SLOT(procesOneThing()));
        timer->start(50000);
    }
}

void Server::slotNewConnection(){
    QTcpSocket* pClientSocket = m_ptcpServer->nextPendingConnection();
        connect(pClientSocket, SIGNAL(disconnected()),
                this, SLOT(slotClientDisconnect())
               );
        connect(pClientSocket, SIGNAL(readyRead()),
                this, SLOT(slotReadClient())
               );
        qDebug()<<"Computer has connected\n";
        sendToClient(pClientSocket, "Server Response: Connected!");
}

void Server::procesOneThing(){
    msql.insertandupdate("update programinrealtime, accounts set programinrealtime.active='0' where programinrealtime.uid=accounts.id and accounts.online='0'");
    QSqlQuery rec1 = msql.select("select enddate, uid from codes where active='1';");
    while (rec1.next()){
    QString res, result1, result2, year, mounth, day, h, m, s;
    res.append(rec1.record().value(0).toString().left(rec1.record().value(0).toString().length()-1).replace(" ", "_"));
    result1.append(res.split("_").first());
    result2.append(res.split("_").last());
    year.append(result1.split("-").at(0));
    mounth.append(result1.split("-").at(1));
    day.append(result1.split("-").at(2));
    h.append(result2.split(":").at(0));
    m.append(result2.split(":").at(1));
    s.append(result2.split(":").at(2));
    QDateTime startdate, enddatet;
    startdate.setDate(QDate::currentDate());
    startdate.setTime(QTime::currentTime());
    QDate enddate; QTime endtime;
    endtime.setHMS(h.toInt(), m.toInt(), s.toInt());
    enddate.setDate(year.toInt(), mounth.toInt(), day.toInt());
    enddatet.setDate(enddate);
    enddatet.setTime(endtime);
    if (startdate>=enddatet){
        msql.insertandupdate("update codes set active=0 where uid='"+rec1.record().value(1).toString()+"';");
    }
    }
    qDebug() << "The database has been successfully cleared from invalid data.\n";
}

void Server::slotClientDisconnect(){
    QTcpSocket* pClientSocket = static_cast<QTcpSocket*>(sender());
    foreach (int key, uid.keys()) {
        if (uid.value(key)==pClientSocket){
            msql.insertandupdate("update accounts set online=0 where id="+QString::number(key)+";");
            uid.remove(key);
        }
    }
    if(adminlst.contains(pClientSocket)){
    adminlst.removeAt(adminlst.indexOf(pClientSocket));
    }
    qDebug()<<"Computer "+ocopms.value(pClientSocket)+" disconnected!\n";
    msql.insertandupdate("update computers set online=0 where compname='"+ocopms.value(pClientSocket)+"';");
    ocopms.remove(pClientSocket);
    pClientSocket->deleteLater();
}
void Server::slotReadClient(){
    QTcpSocket* pClientSocket = static_cast<QTcpSocket*>(sender());
        QDataStream in(pClientSocket);
        for (;;) {
            if (!m_nNextBlockSize) {
                if (pClientSocket->bytesAvailable() < static_cast<unsigned int>(sizeof(quint16))) {
                    break;
                }
                in >> m_nNextBlockSize;
            }

            if (pClientSocket->bytesAvailable() < m_nNextBlockSize) {
                break;
            }
            QString str;
            in >> str;
            QString datat= QDate::currentDate().toString("yyyy-MM-dd");
            QString time= QTime::currentTime().toString("H:m:s a");
            QString dataptime = datat+" "+time;
            if(!(str.contains("/refresh"))){
            msql.insertandupdate("insert into serverstatistics(CommandRecived, TimeRecived) VALUES('"+str+"', '"+datat+"');");
            qDebug()<<"Client has sended - " + str+"\n";
            if (!(adminlst.isEmpty())){
                QString str1 = str;
                str1.replace("/", ".");
                for(int i=0; i<adminlst.length(); i++){
                    sendToClient(adminlst.at(i), "/setserverlist "+ocopms.value(pClientSocket)+" "+str1);
                }
            }
            }
            if(str.contains("/reg")){
                QString resuls=str.replace("\n", "");
                resuls.replace("/reg ","");
                QString login, pass;
                login=resuls.split(" ").first();
                pass=resuls.split(" ").last();
                QSqlQuery rec = msql.select("select * from accounts where accname='"+login+"';");
                rec.first();
                if (msql.getQueryCountIf("accounts", "accname='"+login+"'")==0){
                int count=(msql.getQueryCount("accounts")+1);
                QString result="insert into accounts (id, accname, pass, lastenter, syslvl, online) VALUES("+QString::number(count)+", '"+login+"', '"+pass+"', 'no', 1, 0);";
                msql.insertandupdate(result);
                sendToClient(pClientSocket, "/regsuccess");
                } else {
                    sendToClient(pClientSocket, "Account already exists!");
                }
            } else if(str.contains("/login")){
                QString resuls=str.replace("\n", "");
                resuls.replace("/login ","");
                QString login, pass;
                login=resuls.split(" ").first();
                pass=resuls.split(" ").last();
                QSqlQuery rec = msql.select("select id, syslvl, pass from accounts where accname='"+login+"';");
                rec.first();
                QSqlQuery rec2 = msql.select("select count from accountkicks where uid='"+rec.record().value(0).toString()+"';");
                rec2.first();
                if (rec.record().value(2).toString()==pass){
                    uid.insert(rec.record().value(0).toInt(), pClientSocket);
                    msql.insertandupdate("update programinrealtime set active='0' where uid='"+rec.record().value(0).toString()+"';");
                    if(rec.record().value(1).toInt()==2){
                        adminlst.append(pClientSocket);
                        sendToClient(pClientSocket, "Authorization succes!");
                        sendToClient(pClientSocket, "/setuserid "+rec.record().value(0).toString());
                        sendToClient(pClientSocket, "/setseclvl "+rec.record().value(1).toString());
                        if(!rec2.record().value(0).isNull()){
                        sendToClient(pClientSocket, "/setkickcount "+rec2.record().value(0).toString());
                        } else {
                            sendToClient(pClientSocket, "/setkickcount 0");
                        }
                    } else{
                        sendToClient(pClientSocket, "Authorization succes!");
                        sendToClient(pClientSocket, "/setuserid "+rec.record().value(0).toString());
                        sendToClient(pClientSocket, "/setseclvl "+rec.record().value(1).toString());
                        if(!rec2.record().value(0).isNull()){
                        sendToClient(pClientSocket, "/setkickcount "+rec2.record().value(0).toString());
                        } else {
                            sendToClient(pClientSocket, "/setkickcount 0");
                        }
                        if(msql.getQueryCountIf("codes", "uid='"+rec.record().value(0).toString()+"' and active='1'")==0){
                            sendToClient(pClientSocket, "/openwritecode");
                        } else {
                            QSqlQuery rec1 = msql.select("select enddate from codes where uid='"+rec.record().value(0).toString()+"' and active='1';");
                            rec1.first();
                            QString res, result1, result2, year, mounth, day, h, m, s;
                            res.append(rec1.record().value(0).toString().left(rec1.record().value(0).toString().length()-1).replace(" ", "_"));
                            result1.append(res.split("_").first());
                            result2.append(res.split("_").last());
                            year.append(result1.split("-").at(0));
                            mounth.append(result1.split("-").at(1));
                            day.append(result1.split("-").at(2));
                            h.append(result2.split(":").at(0));
                            m.append(result2.split(":").at(1));
                            s.append(result2.split(":").at(2));
                            QDateTime startdate, enddatet;
                            startdate.setDate(QDate::currentDate());
                            startdate.setTime(QTime::currentTime());
                            QDate enddate; QTime endtime;
                            endtime.setHMS(h.toInt(), m.toInt(), s.toInt());
                            enddate.setDate(year.toInt(), mounth.toInt(), day.toInt());
                            enddatet.setDate(enddate);
                            enddatet.setTime(endtime);
                            if (startdate>=enddatet){
                                msql.insertandupdate("update codes set active=0 where uid='"+rec.record().value(0).toString()+"';");
                                sendToClient(pClientSocket, "/openwritecode");
                            } else {
                                qint64 secs= enddatet.toSecsSinceEpoch()-startdate.toSecsSinceEpoch();
                            sendToClient(pClientSocket, "/1codeistrue1 "+QString::number(secs));
                            sendToClient(pClientSocket, "/setenabledprog chrome");
                            sendToClient(pClientSocket, "/setenabledprog firefox");
                            sendToClient(pClientSocket, "/setenabledprog winrar");
                            sendToClient(pClientSocket, "/setenabledprog wmp");
                            QSqlQuery rec2 = msql.select("select programname from accountprogramban where uid='"+rec.record().value(0).toString()+"';");
                            rec2.first();
                            if(!rec2.record().value(0).isNull()){
                                sendToClient(pClientSocket, "/setdisabledprog "+rec2.record().value(0).toString());
                                while(rec2.next()){
                                    sendToClient(pClientSocket, "/setdisabledprog "+rec2.record().value(0).toString());
                                }
                            }
                            }
                        }
                    }

                    msql.insertandupdate("update computers set lastuid="+QString::number(rec.record().value(0).toInt())+" where compname='"+ocopms.value(pClientSocket)+"';");
                    msql.insertandupdate("update accounts set online=1 where id="+QString::number(rec.record().value(0).toInt())+";");
                    msql.insertandupdate("update accounts set lastenter='"+dataptime+"' where id="+QString::number(rec.record().value(0).toInt())+";");

                } else {
                    sendToClient(pClientSocket, "Authorization failed!");
                }
            } else if (str.contains("/kick")){
                QString resuls=str.replace("\n", "");
                resuls.replace("/kick ","");
                QString res1 = resuls.split(" ").first();
                int usid = res1.toInt();
                QString reason = resuls.split(" ").last();
                if(adminlst.contains(pClientSocket)){
                if(uid.contains(usid)){
                sendToClient(uid.value(usid), "/kickreason "+reason);
                uid.value(usid)->waitForBytesWritten();
                if(!adminlst.contains(uid.value(usid))){
                    msql.insertandupdate("update codes set active=0 where id="+QString::number(usid)+";");
                    QSqlQuery rec1 = msql.select("select count from accountkicks where uid='"+QString::number(usid)+"';");
                    rec1.first();
                    if(rec1.record().value(0).isNull()){
                        msql.insertandupdate("insert into accountkicks(uid, count) values('"+QString::number(usid)+"', '1');");
                    }else {
                        int lastcount=rec1.record().value(0).toInt();
                        lastcount++;
                        msql.insertandupdate("update accountkicks set count="+QString::number(lastcount)+" where uid="+QString::number(usid)+";");
                  }
                } else {
                    QSqlQuery rec1 = msql.select("select count from accountkicks where uid='"+QString::number(usid)+"';");
                    rec1.first();
                    if(rec1.record().value(0).isNull()){
                        msql.insertandupdate("insert into accountkicks(uid, count) values('"+QString::number(usid)+"', '1');");
                    }else {
                        int lastcount=rec1.record().value(0).toInt();
                        lastcount++;
                        msql.insertandupdate("update accountkicks set count="+QString::number(lastcount)+" where uid="+QString::number(usid)+";");
                  }
                }
                } else{
                    sendToClient(pClientSocket, "Id no search!");
                }
                } else {
                    sendToClient(pClientSocket, "Your seclvl is not 2!");
                }
            } else if (str.contains("/loggedof")){
                QString resuls=str.replace("\n", "");
                resuls.replace("/loggedof ","");
                int usid = resuls.toInt();
                msql.insertandupdate("update accounts set online=0 where id="+QString::number(usid)+";");
                if(adminlst.contains(uid.value(usid))){
                    adminlst.removeAt(adminlst.indexOf(uid.value(usid)));
                }
                uid.remove(usid);
                qDebug()<<"Client "+QString::number(usid)+" has logged of\n";
            } else if (str.contains("/checkforfirststart")){
                QString resuls=str.replace("\n", "");
                resuls.replace("/checkforfirststart ","");
                QSqlQuery rec = msql.select("select compname from computers where compname='"+resuls+"';");
                rec.first();
                if (rec.record().value(0).toString().isEmpty()){
                    sendToClient(pClientSocket, "/firststart");
                } else {
                    ocopms.insert(pClientSocket, resuls);
                    msql.insertandupdate("update computers set online=1 where compname='"+resuls+"';");
                }
            } else if (str.contains("/addcomputer")){
                QString resuls=str.replace("\n", "");
                resuls.replace("/addcomputer ","");
                QString compname = resuls.split(" ").at(0);
                QString ip = resuls.split(" ").at(1);
                QString mac = resuls.split(" ").at(2);
                msql.insertandupdate("insert into computers (compname, ip, MAC, lastuid, online) values('"+compname+"','"+ip+"','"+mac+"', NULL, 1);");
                ocopms.insert(pClientSocket, compname);
            } else if (str.contains("/getout")){
                QString resuls=str.replace("\n", "");
                resuls.replace("/getout ","");
                QString compname=resuls.split(" ").first();
                QString msec = resuls.split(" ").last();
                bool comphave = false;
                foreach (QTcpSocket* scok, ocopms.keys()) {
                    if(ocopms.value(scok)==compname){
                       sendToClient(scok, "/getout "+msec);
                       comphave=true;
                    }
                }
                if(!comphave){
                   sendToClient(pClientSocket, "This computer offline");
                }
            } else if(str.contains("/shutdown")){
                QString resuls=str.replace("\n", "");
                resuls.replace("/shutdown ","");
                if(adminlst.contains(pClientSocket)){
                QString compname=resuls.split(" ").first();
                QString msec = resuls.split(" ").last();
                bool comphave = false;
                foreach (QTcpSocket* scok, ocopms.keys()) {
                    if(ocopms.value(scok)==compname){
                       sendToClient(scok, "/shutdown "+msec);
                       comphave=true;
                    }
                }
                if(!comphave){
                   sendToClient(pClientSocket, "This computer offline");
                }
                }else {
                    sendToClient(pClientSocket, "Your seclvl is not 2!");
                }
            } else if(str.contains("/restartcomp")){
                QString resuls=str.replace("\n", "");
                resuls.replace("/restartcomp ","");
                if(adminlst.contains(pClientSocket)){
                QString compname=resuls.split(" ").first();
                QString msec = resuls.split(" ").last();
                bool comphave = false;
                foreach (QTcpSocket* scok, ocopms.keys()) {
                    if(ocopms.value(scok)==compname){
                       sendToClient(scok, "/restartcomp "+msec);
                       comphave=true;
                    }
                }
                if(!comphave){
                   sendToClient(pClientSocket, "This computer offline");
                }
                }else {
                    sendToClient(pClientSocket, "Your seclvl is not 2!");
                }
            } else if (str.contains("/wol")){
                QString resuls=str.replace("\n", "");
                resuls.replace("/wol ","");
                if(adminlst.contains(pClientSocket)){
                    QSqlQuery rec = msql.select("select compname, mac, online from computers where compname='"+resuls+"';");
                    rec.first();
                    if (rec.record().value(0).toString().isEmpty()){
                        sendToClient(pClientSocket, "Comp no search");
                    } else if(rec.record().value(2).toInt()==1) {
                        sendToClient(pClientSocket, "Comp is online");
                    } else {
                        wakeOnLan(rec.record().value(1).toString());
                        sendToClient(pClientSocket, "Comp is up");
                    }
                }else {
                    sendToClient(pClientSocket, "Your seclvl is not 2!");
                }
            } else if(str.contains("/spiscomp")){
                QSqlQuery rec = msql.select("select compname, online from computers;");
                QString resname, resonl, res1, res2;
                resname.append("/setspiscompname ");
                resonl.append("/setspisonline ");
                while(rec.next()){
                    resname.append(rec.record().value(0).toString()+" ");
                    resonl.append(rec.record().value(1).toString()+" ");
                }
                res1.append(resname.left(resname.length()-1));
                res2.append(resonl.left(resonl.length()-1));
                sendToClient(pClientSocket, res1);
                pClientSocket->waitForBytesWritten();
                sendToClient(pClientSocket, res2);
            } else if (str=="/refresh"){
                QSqlQuery rec = msql.select("select compname, online from computers where online=1;");
                QSqlQuery rec2 = msql.select("select accname, syslvl from accounts where online=1;");
                QSqlQuery rec3 = msql.select("select accname from accounts where syslvl=2;");
                QSqlQuery rec4 = msql.select("select accname from accounts;");
                QSqlQuery rec5 = msql.select("select uid, RunProgramName from programinrealtime");
                int componline=0, uonline=0, aonline=0, aacc=0, uacc=0;
                while(rec.next()){
                    componline++;
                }
                while(rec2.next()){
                    uonline++;
                    if(rec2.record().value(1).toString()=="2"){
                        aonline++;
                    }
                }
                while(rec3.next()){
                    aacc++;
                }
                while(rec4.next()){
                    uacc++;
                }
                sendToClient(pClientSocket, "/updcomponline "+QString::number(componline));
                sendToClient(pClientSocket, "/upduseronline "+QString::number(uonline));
                sendToClient(pClientSocket, "/updadminsonline "+QString::number(aonline));
                sendToClient(pClientSocket, "/updalladmins "+QString::number(aacc));
                sendToClient(pClientSocket, "/updallusers "+QString::number(uacc));
                rec4.last();
                sendToClient(pClientSocket, "/updlastuser "+rec4.record().value(0).toString());
                rec5.last();
                sendToClient(pClientSocket, "/updlastprogramanduser "+rec5.record().value(0).toString()+" "+rec5.record().value(1).toString());
            } else if (str=="/usersspisall"){
                QSqlQuery rec = msql.select("select id, accname, lastenter, syslvl, online from accounts;");
                QString resid, resname, reslastenter, ressyslvl, resonline, res1, res2, res3, res4, res5;
                resid.append("/setallusersid ");
                resname.append("/setallusername ");
                reslastenter.append("/setalluserlast ");
                ressyslvl.append("/setallusersyslvl ");
                resonline.append("/setalluseronline ");
                while(rec.next()){
                    resid.append(rec.record().value(0).toString()+" ");
                    resname.append(rec.record().value(1).toString()+" ");
                    reslastenter.append(rec.record().value(2).toString().replace(" ", "_")+" ");
                    ressyslvl.append(rec.record().value(3).toString()+" ");
                    resonline.append(rec.record().value(4).toString()+" ");
                }
                res1.append(resid.left(resid.length()-1));
                res2.append(resname.left(resname.length()-1));
                res3.append(reslastenter.left(reslastenter.length()-1));
                res4.append(ressyslvl.left(ressyslvl.length()-1));
                res5.append(resonline.left(resonline.length()-1));
                sendToClient(pClientSocket, res1);
                pClientSocket->waitForBytesWritten();
                sendToClient(pClientSocket, res2);
                pClientSocket->waitForBytesWritten();
                sendToClient(pClientSocket, res3);
                pClientSocket->waitForBytesWritten();
                sendToClient(pClientSocket, res4);
                pClientSocket->waitForBytesWritten();
                sendToClient(pClientSocket, res5);
            } else if(str=="/spisuserscomp"){
                QSqlQuery rec = msql.select("select a.id, a.accname, c.compname, c.ip from accounts a, computers c where a.id=c.lastuid and a.online=1;");
                QString resid, resname, rescompname, resip, res1, res2, res3, res4;
                resid.append("/setuserscid ");
                resname.append("/setuserscname ");
                rescompname.append("/setusersccname ");
                resip.append("/setuserscip ");
                while(rec.next()){
                    resid.append(rec.record().value(0).toString()+" ");
                    resname.append(rec.record().value(1).toString()+" ");
                    rescompname.append(rec.record().value(2).toString().replace(" ", "_")+" ");
                    resip.append(rec.record().value(3).toString()+" ");
                }
                res1.append(resid.left(resid.length()-1));
                res2.append(resname.left(resname.length()-1));
                res3.append(rescompname.left(rescompname.length()-1));
                res4.append(resip.left(resip.length()-1));
                sendToClient(pClientSocket, res1);
                pClientSocket->waitForBytesWritten();
                sendToClient(pClientSocket, res2);
                pClientSocket->waitForBytesWritten();
                sendToClient(pClientSocket, res3);
                pClientSocket->waitForBytesWritten();
                sendToClient(pClientSocket, res4);
            } else if (str=="/prisuserspis"){
                QSqlQuery rec = msql.select("select id, accname, lastenter, syslvl, online from accounts where online=1;");
                QString resid, resname, resonline, res1, res2, res5;
                resid.append("/setallusersid ");
                resname.append("/setallusername ");
                resonline.append("/setsusersonline ");
                while(rec.next()){
                    resid.append(rec.record().value(0).toString()+" ");
                    resname.append(rec.record().value(1).toString()+" ");
                    resonline.append(rec.record().value(4).toString()+" ");
                }
                res1.append(resid.left(resid.length()-1));
                res2.append(resname.left(resname.length()-1));
                res5.append(resonline.left(resonline.length()-1));
                sendToClient(pClientSocket, res1);
                pClientSocket->waitForBytesWritten();
                sendToClient(pClientSocket, res2);
                pClientSocket->waitForBytesWritten();
                sendToClient(pClientSocket, res5);
            } else if (str.contains("/checkcodetofree")){
                QString resuls=str.replace("\n", "");
                resuls.replace("/checkcodetofree ","");
                QString kod, time;
                kod.append(resuls.split(" ").first());
                time.append(resuls.split(" ").last());
                if(msql.getQueryCountIf("codes", "codeid='"+kod+"'")==1){
                    sendToClient(pClientSocket, "/regencode "+time);
                } else{
                    sendToClient(pClientSocket, "/codefree "+kod);
                    msql.insertandupdate("insert into codes (codeid, enddate, uid, active, time) values('"+kod+"', '', '', '', '"+time+"');");
                }
            } else if(str.contains("/checkontruecode")){
                QString resuls=str.replace("\n", "");
                resuls.replace("/checkontruecode ","");
                if(msql.getQueryCountIf("codes", "codeid='"+resuls+"' and active=''")==1){
                    QSqlQuery rec = msql.select("select time from codes where codeid='"+resuls+"';");
                    rec.first();
                    QDateTime startdatetime, enddatetime;
                    startdatetime.setDate(QDate::currentDate());
                    startdatetime.setTime(QTime::currentTime());
                    enddatetime.setDate(startdatetime.addSecs(rec.record().value(0).toInt()).date());
                    enddatetime.setTime(startdatetime.addSecs(rec.record().value(0).toInt()).time());
                    int idish = 0;
                    foreach (int id, uid.keys()) {
                        if(uid.value(id)==pClientSocket){
                            idish=id;
                        }
                    }
                    QString datat1= enddatetime.date().toString("yyyy-MM-dd");
                    QString time1_1= enddatetime.time().toString("H:m:s a");
                    QString dataptime1 = datat1+" "+time1_1;
                    msql.insertandupdate("update codes set enddate='"+dataptime1+"' where codeid='"+resuls+"';");
                    msql.insertandupdate("update codes set uid='"+QString::number(idish)+"' where codeid='"+resuls+"';");
                    msql.insertandupdate("update codes set active='1' where codeid='"+resuls+"';");
                    sendToClient(pClientSocket, "/codeistrue "+rec.record().value(0).toString());
                    sendToClient(pClientSocket, "/setenabledprog chrome");
                    sendToClient(pClientSocket, "/setenabledprog firefox");
                    sendToClient(pClientSocket, "/setenabledprog winrar");
                    sendToClient(pClientSocket, "/setenabledprog wmp");
                    QSqlQuery rec2 = msql.select("select programname from accountprogramban where uid='"+QString::number(idish)+"';");
                    rec2.first();
                    if(!rec2.record().value(0).isNull()){
                        sendToClient(pClientSocket, "/setdisabledprog "+rec2.record().value(0).toString());
                        while(rec2.next()){
                            sendToClient(pClientSocket, "/setdisabledprog "+rec2.record().value(0).toString());
                        }
                    }
                } else {
                    sendToClient(pClientSocket, "/codenottrue");
                }
            } else if(str.contains("/addtimetouser")){
                QString resuls=str.replace("\n", "");
                resuls.replace("/addtimetouser ","");
                QString uid2, addedsecs;
                uid2.append(resuls.split(" ").first());
                addedsecs.append(resuls.split(" ").last());
                QSqlQuery rec1 = msql.select("select enddate, time from codes where uid='"+uid2+"' and active='1';");
                rec1.first();
                if (rec1.record().value(0).toString().isEmpty()){
                    sendToClient(pClientSocket, "Id no search!");
                } else {
                QString res, result1, result2, year, mounth, day, h, m, s, oldtime=rec1.record().value(1).toString();
                res.append(rec1.record().value(0).toString().left(rec1.record().value(0).toString().length()-1).replace(" ", "_"));
                result1.append(res.split("_").first());
                result2.append(res.split("_").last());
                year.append(result1.split("-").at(0));
                mounth.append(result1.split("-").at(1));
                day.append(result1.split("-").at(2));
                h.append(result2.split(":").at(0));
                m.append(result2.split(":").at(1));
                s.append(result2.split(":").at(2));
                QDateTime enddate;
                enddate.setDate(QDate(year.toInt(), mounth.toInt(), day.toInt()));
                enddate.setTime(QTime(h.toInt(), m.toInt(), s.toInt()));
                enddate=enddate.addSecs(addedsecs.toInt());
                QString datat1= enddate.date().toString("yyyy-MM-dd");
                QString time1_1= enddate.time().toString("H:m:s a");
                QString dataptime1 = datat1+" "+time1_1;
                QDateTime startdate, enddatet;
                startdate.setDate(QDate::currentDate());
                startdate.setTime(QTime::currentTime());
                QDate enddate1; QTime endtime1;
                endtime1.setHMS(enddate.time().hour(), enddate.time().minute(), enddate.time().second());
                enddate1.setDate(enddate.date().year(), enddate.date().month(), enddate.date().day());
                enddatet.setDate(enddate1);
                enddatet.setTime(endtime1);
                msql.insertandupdate("update codes set enddate='"+dataptime1+"' where uid='"+uid2+"';");
                msql.insertandupdate("update codes set time='"+QString::number(oldtime.toInt()+addedsecs.toInt())+"' where uid='"+uid2+"';");
                if (uid.keys().contains(uid2.toInt())){
                    sendToClient(uid.value(uid2.toInt()), "/updatecodetime "+QString::number(startdate.secsTo(enddatet)));
                }
                sendToClient(pClientSocket, "/timeadded");
                }
            } else if(str.contains("/programopened")){
                QString resuls=str.replace("\n", "");
                resuls.replace("/programopened ","");
                QString uid, pname;
                uid.append(resuls.split(" ").first());
                pname.append(resuls.split(" ").last());
                int idnomer=(msql.getQueryCount("programinrealtime")+1);
                msql.insertandupdate("insert into programinrealtime (id, RunProgramName, RunDataTime, active, uid) values('"+QString::number(idnomer)+"','"+pname+"','"+dataptime.left(dataptime.length()-1)+"','1','"+uid+"');");
            } else if(str.contains("/programclosed")){
                QString resuls=str.replace("\n", "");
                resuls.replace("/programclosed ","");
                QString uid, pname;
                uid.append(resuls.split(" ").first());
                pname.append(resuls.split(" ").last());
                msql.insertandupdate("update programinrealtime set active=0 where uid='"+uid+"' and active='1' and RunProgramName='"+pname+"';");
            } else if (str.contains("/changepass")){
                QString resuls=str.replace("\n", "");
                resuls.replace("/changepass ","");
                QString uid, oldpas, newpas;
                uid.append(resuls.split(" ").at(0));
                oldpas.append(resuls.split(" ").at(1));
                newpas.append(resuls.split(" ").at(2));
                QString oldmsqlpas;
                QSqlQuery rec = msql.select("select pass from accounts where id='"+uid+"';");
                rec.first();
                oldmsqlpas.append(rec.record().value(0).toString());
                if (!(oldmsqlpas==oldpas)){
                    sendToClient(pClientSocket, "Change pass incorrect!");
                } else {
                    msql.insertandupdate("update accounts set pass='"+newpas+"' where id='"+uid+"';");
                    sendToClient(pClientSocket, "Password change");
                }
            } else if (str.left(15)=="/addmesstoadmin"){
                QString resuls=str.left(str.length()-1);
                resuls.replace("/addmesstoadmin '","");
                QStringList nicklist, idlist;
                int randres=0, mesid=0;
                QSqlQuery rec = msql.select("select accname, id from accounts where syslvl='2';");
                int adcount=0;
                while(rec.next()){
                    nicklist.insert(adcount, rec.record().value(0).toString());
                    idlist.insert(adcount, rec.record().value(1).toString());
                    adcount++;
                }
                randres=randomBetween(0, adcount-1, static_cast<int>(QDateTime::currentDateTime().currentMSecsSinceEpoch()));
                mesid=msql.getQueryCount("messagetoadmin;")+1;
                QString resid = idlist.at(randres);
                if(adminlst.contains(uid.value(resid.toInt()))){
                    qDebug() << "Admin " <<nicklist.at(randres)<< " online and have new message";
                    resuls.replace("'", "\\'");
                    msql.insertandupdate("insert into messagetoadmin (mid, uid, adminid, message, compname, response, active) values('"+QString::number(mesid)+"', '', '"+resid+"', '"+resuls.replace("\n", "@enter@")+"', '"+ocopms.value(pClientSocket)+"', '', '1');");
                    int newmescount = msql.getQueryCountIf("messagetoadmin", "adminid='"+resid+"' and response='' and active='1'");
                    sendToClient(uid.value(resid.toInt()), "You have new mes");
                    sendToClient(uid.value(resid.toInt()), "/setnewadminmes "+QString::number(newmescount));
                    sendToClient(pClientSocket, "/messagesended "+nicklist.at(randres));
                } else {
                    resuls.replace("'", "\\'");
                    msql.insertandupdate("insert into messagetoadmin (mid, uid, adminid, message, compname, response, active) values('"+QString::number(mesid)+"', '', '"+resid+"', '"+resuls.replace("\n", "@enter@")+"', '"+ocopms.value(pClientSocket)+"', '', '1');");
                    sendToClient(pClientSocket, "/messagesended "+nicklist.at(randres));
                }
            } else if (str.contains("/addadminmesslogged")){
                QString resuls=str.left(str.length()-1);
                resuls.replace("/addadminmesslogged ","");
                QString uid2=resuls.split(" ").at(0);
                resuls= resuls.right(resuls.length()-(uid2.length()+2));
                QStringList nicklist, idlist;
                int randres=0, mesid=0;
                QSqlQuery rec = msql.select("select accname, id from accounts where syslvl='2';");
                int adcount=0;
                while(rec.next()){
                    nicklist.insert(adcount, rec.record().value(0).toString());
                    idlist.insert(adcount, rec.record().value(1).toString());
                    adcount++;
                }
                randres=randomBetween(0, adcount-1, static_cast<int>(QDateTime::currentDateTime().currentMSecsSinceEpoch()));
                mesid=msql.getQueryCount("messagetoadmin;")+1;
                QString resid = idlist.at(randres);
                if(adminlst.contains(uid.value(resid.toInt()))){
                    qDebug() << "Admin " <<nicklist.at(randres)<< " online and have new message";
                    resuls.replace("'", "\\'");
                    msql.insertandupdate("insert into messagetoadmin (mid, uid, adminid, message, compname, response, active) values('"+QString::number(mesid)+"', '"+uid2+"', '"+resid+"', '"+resuls.replace("\n", "@enter@")+"', '"+ocopms.value(pClientSocket)+"', '', '1');");
                    int newmescount = msql.getQueryCountIf("messagetoadmin", "adminid='"+resid+"' and response='' and active='1'");
                    sendToClient(uid.value(resid.toInt()), "You have new mes");
                    sendToClient(uid.value(resid.toInt()), "/setnewadminmes "+QString::number(newmescount));
                    sendToClient(pClientSocket, "/messagesended "+nicklist.at(randres));
                } else {
                    resuls.replace("'", "\\'");
                    msql.insertandupdate("insert into messagetoadmin (mid, uid, adminid, message, compname, response, active) values('"+QString::number(mesid)+"', '"+uid2+"', '"+resid+"', '"+resuls.replace("\n", "@enter@")+"', '"+ocopms.value(pClientSocket)+"', '', '1');");
                    sendToClient(pClientSocket, "/messagesended "+nicklist.at(randres));
                }
            } else if (str.contains("/1addmesstoadmin")){
                QString resuls=str.left(str.length()-1);
                resuls.replace("/1addmesstoadmin '","");
                QStringList nicklist, idlist;
                int randres=0, mesid=0;
                QSqlQuery rec = msql.select("select accname, id from accounts where syslvl='2' and online='1';");
                int adcount=0;
                while(rec.next()){
                    nicklist.insert(adcount, rec.record().value(0).toString());
                    idlist.insert(adcount, rec.record().value(1).toString());
                    adcount++;
                }
                if(!(adcount==0)){
                randres=randomBetween(0, adcount-1, static_cast<int>(QDateTime::currentDateTime().currentMSecsSinceEpoch()));
                mesid=msql.getQueryCount("messagetoadmin;")+1;
                QString resid = idlist.at(randres);
                if(adminlst.contains(uid.value(resid.toInt()))){
                    qDebug() << "Admin " <<nicklist.at(randres)<< " online and have new message";
                    resuls.replace("'", "\\'");
                    msql.insertandupdate("insert into messagetoadmin (mid, uid, adminid, message, compname, response, active) values('"+QString::number(mesid)+"', '', '"+resid+"', '"+resuls.replace("\n", "@enter@")+"', '"+ocopms.value(pClientSocket)+"', '', '1');");
                    int newmescount = msql.getQueryCountIf("messagetoadmin", "adminid='"+resid+"' and response='' and active='1'");
                    sendToClient(uid.value(resid.toInt()), "You have new mes");
                    sendToClient(uid.value(resid.toInt()), "/setnewadminmes "+QString::number(newmescount));
                    sendToClient(pClientSocket, "/messagesended "+nicklist.at(randres));
                }
                } else {
                    sendToClient(pClientSocket, "Admin not online");
                }
            } else if (str.contains("/1addadminmesslogged")){
                QString resuls=str.left(str.length()-1);
                resuls.replace("/1addadminmesslogged ","");
                QString uid2=resuls.split(" ").at(0);
                resuls= resuls.right(resuls.length()-(uid2.length()+2));
                QStringList nicklist, idlist;
                int randres=0, mesid=0;
                QSqlQuery rec = msql.select("select accname, id from accounts where syslvl='2' and online='1';");
                int adcount=0;
                while(rec.next()){
                    nicklist.insert(adcount, rec.record().value(0).toString());
                    idlist.insert(adcount, rec.record().value(1).toString());
                    adcount++;
                }
                if(!(adcount==0)){
                randres=randomBetween(0, adcount-1, static_cast<int>(QDateTime::currentDateTime().currentMSecsSinceEpoch()));
                mesid=msql.getQueryCount("messagetoadmin;")+1;
                QString resid = idlist.at(randres);
                if(adminlst.contains(uid.value(resid.toInt()))){
                    qDebug() << "Admin " <<nicklist.at(randres)<< " online and have new message";
                    resuls.replace("'", "\\'");
                    msql.insertandupdate("insert into messagetoadmin (mid, uid, adminid, message, compname, response, active) values('"+QString::number(mesid)+"', '"+uid2+"', '"+resid+"', '"+resuls.replace("\n", "@enter@")+"', '"+ocopms.value(pClientSocket)+"', '', '1');");
                    int newmescount = msql.getQueryCountIf("messagetoadmin", "adminid='"+resid+"' and response='' and active='1'");
                    sendToClient(uid.value(resid.toInt()), "You have new mes");
                    sendToClient(uid.value(resid.toInt()), "/setnewadminmes "+QString::number(newmescount));
                    sendToClient(pClientSocket, "/messagesended "+nicklist.at(randres));
                }
                } else {
                    sendToClient(pClientSocket, "Admin not online");
                }
            } else if(str.contains("/chmessage")){
                QString result=str.replace("\n", "");
                result.replace("/chmessage ", "");
                if (adminlst.contains(uid.value(result.toInt()))){
                    int newmescount = msql.getQueryCountIf("messagetoadmin", "adminid='"+result+"' and response='' and active='1'");
                    sendToClient(pClientSocket, "/setnewadminmes "+QString::number(newmescount));
                    QSqlQuery rec = msql.select("select response from messagetoadmin where uid='"+result+"' and active='1';");
                    int newrescount=0;
                    while(rec.next()){
                        if(!(rec.record().value(0).toString()=="")){
                            newrescount++;
                        }
                    }
                    int allqcount = msql.getQueryCountIf("messagetoadmin", "uid='"+result+"'");
                    sendToClient(pClientSocket, "/setnewmesinf "+QString::number(newrescount)+" "+QString::number(allqcount));
                } else {
                    QSqlQuery rec = msql.select("select response from messagetoadmin where uid='"+result+"' and active='1';");
                    int newrescount=0;
                    while(rec.next()){
                        if(!(rec.record().value(0).toString()=="")){
                            newrescount++;
                        }
                    }
                    int allqcount = msql.getQueryCountIf("messagetoadmin", "uid='"+result+"'");
                    sendToClient(pClientSocket, "/setnewmesinf "+QString::number(newrescount)+" "+QString::number(allqcount));
                }
            } else if(str.contains("/getusermes")){
                QString result=str.replace("\n", "");
                result.replace("/getusermes ", "");
                QSqlQuery rec = msql.select("select response, message, mid, active from messagetoadmin where uid='"+result+"';");
                int rescount=0;
                while(rec.next()){
                    sendToClient(pClientSocket, "/setusermes "+QString::number(rescount)+" "+rec.record().value(2).toString()+" "+rec.record().value(3).toString()+" '"+rec.record().value(1).toString().replace("@enter@", "\n")+"' '"+rec.record().value(0).toString().replace("@enter@", "\n")+"'");
                    rescount++;
                    pClientSocket->waitForBytesWritten();
                }
            } else if(str.contains("/setseemsg")){
                QString result=str.replace("\n", "");
                result.replace("/setseemsg ", "");
                QString mid, usrid;
                mid.append(result.split(" ").first());
                usrid.append(result.split(" ").last());
                msql.insertandupdate("update messagetoadmin set active='0' where mid='"+mid+"' and uid='"+usrid+"';");
            } else if(str.contains("/getadminmes")){
                QString result=str.replace("\n", "");
                result.replace("/getadminmes ", "");
                QSqlQuery rec = msql.select("select mid, uid, message, compname from messagetoadmin where adminid='"+result+"' and response='' and active='1'");
                int rescount=0;
                while(rec.next()){
                    sendToClient(pClientSocket, "/setadminmes "+QString::number(rescount)+" "+rec.record().value(0).toString()+" "+rec.record().value(1).toString()+" "+rec.record().value(3).toString()+" '"+rec.record().value(2).toString().replace("@enter@", "\n")+"'");
                    rescount++;
                    pClientSocket->waitForBytesWritten();
                }
            } else if (str.contains("/setresponsemsg")){
                QString result=str.replace("\n", "");
                result.replace("/setresponsemsg ", "");
                QString mid, admuid, response;
                mid.append(result.split(" ").first());
                admuid.append(result.split(" ").at(1));
                result = result.left(result.length()-1);
                result.replace(mid+ " "+admuid+ " '", "");
                response = result;
                response.replace("\n", "@enter@");
                msql.insertandupdate("update messagetoadmin set response='"+response.replace("'", "\\'")+"' where mid='"+mid+"' and uid='"+admuid+"';");
                if(uid.contains(admuid.toInt())){
                    sendToClient(uid.value(admuid.toInt()), "/newresponse");
                }
            } else if (str.contains("/setcompresponsemsg")){
                QString result=str.replace("\n", "");
                result.replace("/setcompresponsemsg ", "");
                QString mid, comp, response;
                mid.append(result.split(" ").first());
                comp.append(result.split(" ").at(1));
                result = result.left(result.length()-1);
                result.replace(mid+ " "+comp+ " '", "");
                response = result;
                response.replace("\n", "@enter@");
                msql.insertandupdate("update messagetoadmin set response='"+response.replace("'", "\\'")+"' where mid='"+mid+"' and compname='"+comp+"';");
                bool coonl=false;
                for(int i=0; i<ocopms.keys().length(); i++){
                    if (ocopms.value(ocopms.keys().at(i))==comp){
                        QSqlQuery rec = msql.select("select message from messagetoadmin where mid='"+mid+"';");
                        rec.first();
                        sendToClient(ocopms.keys().at(i), "/newcompresponse "+mid+ " '"+rec.record().value(0).toString().replace("@enter@", "\n")+"' '"+response.replace("@enter@", "\n")+"'");
                        coonl=true;
                        break;
                    }
                }
                if(!coonl){
                    msql.insertandupdate("update messagetoadmin set active='0' where mid='"+mid+"' and compname='"+comp+"';");
                }
            } else if (str.contains("/setcompsee")){
                QString result=str.replace("\n", "");
                result.replace("/setcompsee ", "");
                QString mid= result;
                msql.insertandupdate("update messagetoadmin set active='0' where mid='"+mid+"';");
            } else if(str.contains("/disableprogram")){
                QString result=str.replace("\n", "");
                result.replace("/disableprogram ", "");
                QString userprid = result.split(" ").first();
                QString pname=result.split(" ").last();
                QSqlQuery rec = msql.select("select programname from accountprogramban where uid='"+userprid+"' and programname='"+pname+"';");
                rec.first();
                if(rec.record().value(0).isNull()){
                   msql.insertandupdate("insert into accountprogramban(uid, programname) values('"+userprid+"', '"+pname+"');");
                   if(uid.contains(userprid.toInt())){
                       sendToClient(uid.value(userprid.toInt()), "/setdisabledprog "+pname);
                   }
                } else {
                    sendToClient(pClientSocket, "Program already banned");
                }
            } else if(str.contains("/enableprogram")){
                QString result=str.replace("\n", "");
                result.replace("/enableprogram ", "");
                QString userprid = result.split(" ").first();
                QString pname=result.split(" ").last();
                QSqlQuery rec = msql.select("select programname from accountprogramban where uid='"+userprid+"' and programname='"+pname+"';");
                rec.first();
                if(!rec.record().value(0).isNull()){
                   msql.insertandupdate("delete from accountprogramban where uid='"+userprid+"' and programname='"+pname+"';");
                   if(uid.contains(userprid.toInt())){
                    sendToClient(uid.value(userprid.toInt()), "/setenabledprog "+pname);
                   }
                } else {
                    sendToClient(pClientSocket, "Program dont have");
                }
            } else if(str.contains("/openprogram")){
                QString result=str.replace("\n", "");
                result.replace("/openprogram ", "");
                QString userprid = result.split(" ").first();
                QString pname=result.split(" ").last();
                QSqlQuery rec = msql.select("select RunProgramName from programinrealtime where uid="+userprid+" and active=1;");
                rec.first();
                if(rec.record().value(0).isNull()){
                    if(uid.contains(userprid.toInt())){
                    sendToClient(uid.value(userprid.toInt()), "/openthisprog "+pname);
                    }
                } else {
                    sendToClient(pClientSocket, "Program already open");
                }
            } else if(str.contains("/closeprogram")){
                QString result=str.replace("\n", "");
                result.replace("/closeprogram ", "");
                QString userprid = result.split(" ").first();
                QString pname=result.split(" ").last();
                QSqlQuery rec = msql.select("select RunProgramName from programinrealtime where uid="+userprid+" and active=1;");
                rec.first();
                if(!rec.record().value(0).isNull()){
                    if(uid.contains(userprid.toInt())){
                    sendToClient(uid.value(userprid.toInt()), "/closethisprog "+pname);
                    }
                } else {
                    sendToClient(pClientSocket, "Program dont open");
                }
            }

            m_nNextBlockSize = 0;
        }
}

int Server::randomBetween(int low, int high, int seed){
    qsrand(static_cast<uint>(seed)); // Установка базового числа для отсчёта рандома в qrand
    return (qrand() % ((high + 1) - low) + low);
}

void Server::wakeOnLan(QString MAC)
{
    char MACAddr [6];
    char MagicPacket [102]; // Magic package for remote boot
    MAC.replace(":", "-");
    int j = sscanf_s(MAC.toLatin1().data(), "%2x-%2x-%2x-%2x-%2x-%2x",
                    & MACAddr [0], & MACAddr [1], & MACAddr [2], & MACAddr [3], & MACAddr [4], & MACAddr [5]);

    // Set to hexadecimal before the magicpacket array 6 characters ff
    memset (MagicPacket, 0xff, 6);

    int packetsize = 6; // the beginning of the initial value is 6, do not wrong. I is because the effect of the initial value of the written as 0, it is too effortless.
    // Build MagicPacket.
    for (int i = 0; i <16; i++)
    {
        memcpy (MagicPacket + packetsize, MACAddr, 6);
        packetsize += 6;
    }

    QHostAddress FakeAddress;
    FakeAddress.setAddress ("192.168.1.255");

    QUdpSocket udpSocket;
    udpSocket.writeDatagram(MagicPacket, 102, FakeAddress, 9);
    Q_UNUSED(j);
}

void Server::sendToClient(QTcpSocket *pSocket, const QString &str){
    QByteArray  arrBlock;
        QDataStream out(&arrBlock, QIODevice::WriteOnly);
        out << quint16(0) << str;

        out.device()->seek(0);
        out << quint16(static_cast<quint16>(arrBlock.size()) - sizeof(quint16));

        pSocket->write(arrBlock);
}
