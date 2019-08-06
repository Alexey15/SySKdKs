#ifndef MYCLIENT_H
#define MYCLIENT_H

#include <QWidget>
#include <QMainWindow>
#include <QDir>
#include <QListWidget>
#include <QTcpSocket>
#include <QMovie>
#include <QPixmap>
#include <QBitmap>
#include <QPushButton>
#include <QProcess>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPixmap>
#include <QHostInfo>
#include <QNetworkInterface>
#include <QTimer>
#include <QDateTime>
#include <QDate>
#include <QGraphicsEffect>
#include <QDialog>
#include <QMessageBox>
#include <QLabel>
#include <QTableWidget>
#include <QTime>
#include "progfunkcii.h"
#include "popup.h"
#include <QTextEdit>
#include <QLineEdit>
#include <QCheckBox>

extern QWidget *tabs2;
namespace Ui {
class MyClient;
}

class MyClient : public QMainWindow
{
    Q_OBJECT

public:
    explicit MyClient(const QString& strHost, quint16 nPort, QWidget *parent = 0);
    ~MyClient();

private:
    Ui::MyClient *ui;
    QTcpSocket* m_pTcpSocket;
    QString host, compmid;
    QStringList usrmsgresp, usrmsg, usrmsgact, usrmid;
    QStringList admmsgcomp, admmsg, admmsguid, admmid;
    int time=14, countofrow=0, idnomer, idnomer2, countofrow2=0;
    quint16 port;
    QDialog *dial;
    QDialog *dial2;
    QTimer *timer1;
    QTimer *timer2;
    QTimer *timer3;
    QTimer *reftimer;
    QLabel *gllab;
    QTableWidget *tblewid, *tblewid2;
    QDialog *dial3;
    QDialog *dial4;
    QDialog *dial5;
    QDialog *dial6;
    QDialog *dial7;
    QDialog *dial8;
    QDialog *dial9;
    QDialog *dial10;
    QDialog *dial11;
    QDialog *dial12;
    QDialog *dial13;
    QCheckBox *chb;
    QLineEdit *ledit;
    QLineEdit *ledit2;
    QTextEdit *tedit, *admreste;
    QLineEdit *ledcode;
    QLineEdit *lineed;
    QLineEdit *lineed2;
    QLineEdit *lineed3;
    QPropertyAnimation *anim;
    QPropertyAnimation *anim2;
    bool trues=true, trues2=true, trues3=true, trues4=true, trues5=true, trues6=true, trues7=true, logged=false, kick=false;
    //Programs
    progfunkcii *firefox;
    progfunkcii *chrome;
    progfunkcii *explorer;
    progfunkcii *wmp;
    //Programs
    QLineEdit *ledit3;
    QLineEdit *ledit4;
    QString nickname;
    QMovie *mov;
    QStringList compsname;
    QStringList compsonline;
    QStringList usersid;
    QStringList usersname;
    QStringList userslastenter;
    QStringList userssyslvl;
    QStringList usersonline;
    QStringList userscid;
    QStringList userscname;
    QStringList usersccname;
    QStringList userscip;
    QListWidget *lstwid;
    QListWidget *lstwid2;
    QListWidget *uslistwid;
    QListWidget *uslistwid2;
    QListWidget *uslistwid3;
    QListWidget *uslistwid4;
    QListWidget *uslistwid5;
    QListWidget *userlistwid;
    QListWidget *userlistwid2;
    QListWidget *userlistwid5;
    QListWidget *usclistwid;
    QListWidget *usclistwid2;
    QListWidget *usclistwid3;
    QListWidget *usclistwid4;
    int mySecLVL, myID, hh=0, mm=3, sec=0, second=0;
    void slotSendToServer(QString pov);
    QString getMacAddress();
    int convertToSec(int h, int m, int s);
    int SecToH(int s);
    int SecToM(int s);
    int SecToS(int s);
    int randomBetween(int low, int high, uint seed);
    quint16     m_nNextBlockSize;
private slots:
    void slotReadyRead   (                            );
    void slotError       (QAbstractSocket::SocketError);
    void slotConnected   (                            );
    void on_pushButton_clicked();
    void pb1Clicked();
    void pb2Clicked();
    void pb3Clicked();
    void pb4Clicked();
    void pb5Clicked();
    void pb6Clicked();
    void dial10finish(int kod);
    void dial12finish(int kod);
    void dial13finish(int kod);
    void codepb1push();
    void checkSpace(QString str);
    void checkSpace2(QString str);
    void checkSpace3(QString str);
    void checkSpace4(QString str);
    void checkSpace5(QString str);
    void checkSpace6(QString str);
    void checkSpace7(QString str);
    void Prog1Close();
    void Prog2Close();
    void Prog3Close();
    void Prog4Close();
    void Prog1Open();
    void Prog2Open();
    void Prog3Open();
    void Prog4Open();
    void CloseAllOpenPrograms();
    void timerupd();
    void timerupd2();
    void timerupd3();
    void timerupd4();
    void timerupd5();
    void timerupd6();
    void timerupd7();
    void slotPb1click();
    void rowchangedcspis(int row);
    void rowchangedcspis2(int row);
    void rowchangeduspis(int row);
    void rowchangeduspis2(int row);
    void rowchangeduspis3(int row);
    void rowchangeduspis4(int row);
    void rowchangeduspis5(int row);
    void rowchangeduscspis(int row);
    void rowchangeduscspis2(int row);
    void rowchangeduscspis3(int row);
    void rowchangeduscspis4(int row);
    void rowchangeduserspis(int row);
    void rowchangeduserspis2(int row);
    void rowchangeduserspis3(int row);
    void on_pushButton_2_clicked();
    void on_pushButton_3_clicked();
    void on_pushButton_5_clicked();
    void on_pushButton_6_clicked();
    void on_pushButton_4_clicked();
    void on_pushButton_7_clicked();
    void on_pushButton_8_clicked();
    void lstobr();
    void processOneThing();
    void on_pushButton_14_clicked();
    void on_pushButton_12_clicked();
    void on_pushButton_9_clicked();
    void on_pushButton_11_clicked();
    void on_pushButton_13_clicked();
    void on_pushButton_15_clicked();
    void on_pushButton_10_clicked();
    void on_pushButton_16_clicked();
    void on_pushButton_18_clicked();
    void on_pushButton_17_clicked();
    void on_pushButton_19_clicked();
    void on_pushButton_20_clicked();
    void on_pushButton_21_clicked();
    void on_pushButton_22_clicked();
    void on_pushButton_23_clicked();
    void on_pushButton_24_clicked();
    void admresteclck();
};

#endif // MYCLIENT_H
