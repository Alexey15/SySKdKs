#include "myclient.h"
#include "ui_myclient.h"

QWidget *tabs2;
MyClient::MyClient(const QString &strHost, quint16 nPort, QWidget *parent) :
    QMainWindow(parent), ui(new Ui::MyClient), m_nNextBlockSize(0)
{
    ui->setupUi(this);
    setWindowFlags(Qt::WindowStaysOnBottomHint | Qt::WindowMinimizeButtonHint);
    showFullScreen();
    m_pTcpSocket = new QTcpSocket(this);
            host= strHost;
            port=nPort;
            m_pTcpSocket->connectToHost(strHost, nPort);
            connect(m_pTcpSocket, SIGNAL(connected()), SLOT(slotConnected()));
            connect(m_pTcpSocket, SIGNAL(readyRead()), SLOT(slotReadyRead()));
            connect(m_pTcpSocket, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(slotError(QAbstractSocket::SocketError)));

    mov = new QMovie(QDir::currentPath()+"/2.ren");
    QSize siz;
    siz.setHeight(763);
    siz.setWidth(1363);
    mov->setScaledSize(siz);
    ui->label_40->setMovie(mov);
    ui->label_40->resize(1363, 768);
    ui->label_41->setMovie(mov);
    ui->label_41->resize(1363, 768);
    ui->lineEdit->setValidator( new QIntValidator(0, 1000000, this) );
    ui->lineEdit_9->setValidator( new QIntValidator(0, 1000000, this) );
    ui->lineEdit_3->setValidator( new QIntValidator(0, 1000000, this) );
    ui->lineEdit_4->setValidator( new QIntValidator(0, 1000000, this) );
    ui->lineEdit_6->setValidator( new QIntValidator(0, 1000000, this) );
    ui->lineEdit_7->setValidator( new QIntValidator(0, 1000000, this) );
    ui->lineEdit_8->setValidator( new QIntValidator(0, 1000000, this) );
    mov->start();
    QStringList arg;
    ui->textEdit->setReadOnly(true);
    //
    firefox = new progfunkcii("Mozilla Firefox", QDir::rootPath()+"/Program Files/Mozilla Firefox/firefox.exe", arg);
    ui->verticalLayout_2->addWidget(firefox);
    firefox->setSS("QPushButton:disabled{\n	color: rgb(170, 170, 127);\n	background-color: rgb(170, 170, 127, 60);\n}\nQPushButton:enabled{\n	background-color: rgb(255, 255, 255, 100);\n}");
    firefox->setEnabled(false);
    connect(firefox, SIGNAL(ProgramClosed()), this, SLOT(Prog1Close()));
    connect(firefox, SIGNAL(ProgramOpened()), this, SLOT(Prog1Open()));
    QPixmap pix("./programsicon/firefox.png");
    pix=pix.scaled(ui->label_42->width(), ui->label_42->height());
    ui->label_42->setPixmap(pix);
    //
    chrome = new progfunkcii("Google Chrome", QDir::rootPath()+"Program Files (x86)/Google/Chrome/Application/chrome.exe", arg);
    ui->verticalLayout_3->addWidget(chrome);
    chrome->setSS("QPushButton:disabled{\n	color: rgb(170, 170, 127);\n	background-color: rgb(170, 170, 127, 60);\n}\nQPushButton:enabled{\n	background-color: rgb(255, 255, 255, 100);\n}");
    chrome->setEnabled(false);
    connect(chrome, SIGNAL(ProgramClosed()), this, SLOT(Prog2Close()));
    connect(chrome, SIGNAL(ProgramOpened()), this, SLOT(Prog2Open()));
    QPixmap pix1("./programsicon/chrome.png");
    pix1=pix1.scaled(ui->label_49->width(), ui->label_49->height());
    ui->label_49->setPixmap(pix1);
    //
    explorer = new progfunkcii("WinRar", QDir::rootPath()+"/Program Files/WinRAR/winrar.exe", arg);
    ui->verticalLayout_4->addWidget(explorer);
    explorer->setSS("QPushButton:disabled{\n	color: rgb(170, 170, 127);\n	background-color: rgb(170, 170, 127, 60);\n}\nQPushButton:enabled{\n	background-color: rgb(255, 255, 255, 100);\n}");
    explorer->setEnabled(false);
    connect(explorer, SIGNAL(ProgramClosed()), this, SLOT(Prog3Close()));
    connect(explorer, SIGNAL(ProgramOpened()), this, SLOT(Prog3Open()));
    QPixmap pix2("./programsicon/winrar.png");
    pix2=pix2.scaled(ui->label_50->width(), ui->label_50->height());
    ui->label_50->setPixmap(pix2);
    //Windows Media Player
    wmp = new progfunkcii("Windows Media Player", QDir::rootPath()+"Program Files (x86)/Windows Media Player/wmplayer.exe", arg);
    ui->verticalLayout_5->addWidget(wmp);
    wmp->setSS("QPushButton:disabled{\n	color: rgb(170, 170, 127);\n	background-color: rgb(170, 170, 127, 60);\n}\nQPushButton:enabled{\n	background-color: rgb(255, 255, 255, 100);\n}");
    wmp->setEnabled(false);
    connect(wmp, SIGNAL(ProgramClosed()), this, SLOT(Prog4Close()));
    connect(wmp, SIGNAL(ProgramOpened()), this, SLOT(Prog4Open()));
    QPixmap pix3("./programsicon/wmp.png");
    pix3=pix3.scaled(ui->label_51->width(), ui->label_51->height());
    ui->label_51->setPixmap(pix3);
    //
    tabs2 = ui->tab_2;
    ui->tabWidget->removeTab(1);
    timer2 = new QTimer;
    timer3 = new QTimer;
}
int MyClient::randomBetween(int low, int high, uint seed)
{
    qsrand(seed);
    return (qrand() % ((high + 1) - low) + low);
}
void MyClient::Prog1Close(){
    slotSendToServer("/programclosed "+ QString::number(myID) +" firefox");
}
void MyClient::Prog2Close(){
    slotSendToServer("/programclosed "+ QString::number(myID) +" chrome");
}
void MyClient::Prog3Close(){
    slotSendToServer("/programclosed "+ QString::number(myID) +" winrar");
}
void MyClient::Prog4Close(){
    slotSendToServer("/programclosed "+ QString::number(myID) +" wmp");
}
void MyClient::Prog1Open(){
    slotSendToServer("/programopened "+ QString::number(myID) + " firefox");
}
void MyClient::Prog2Open(){
    slotSendToServer("/programopened "+ QString::number(myID) + " chrome");
}
void MyClient::Prog3Open(){
    slotSendToServer("/programopened "+ QString::number(myID) + " winrar");
}
void MyClient::Prog4Open(){
    slotSendToServer("/programopened "+ QString::number(myID) + " wmp");
}

void MyClient::slotReadyRead(){
    QDataStream in(m_pTcpSocket);
        for (;;) {
            if (!m_nNextBlockSize) {
                if (m_pTcpSocket->bytesAvailable() < static_cast<unsigned int>(sizeof(quint16))) {
                    break;
                }
                in >> m_nNextBlockSize;
            }

            if (m_pTcpSocket->bytesAvailable() < m_nNextBlockSize) {
                break;
            }
            QString str;
            in >> str;
            if(str.contains("/setseclvl")){
                logged=true;
                ui->pushButton_2->setText("Вийти");
                QString resuls = str;
                resuls.replace("/setseclvl ", "");
                mySecLVL = resuls.toInt();
                ui->label_4->setText("Онлайн");
                ui->label_4->setStyleSheet("color: rgb(0, 255, 0)");
                if(mySecLVL==2){
                    ui->tabWidget->addTab(tabs2, "Панель адміністратора");
                    ui->label_13->setText("Адміністратор");
                    firefox->setEnabled(true);
                    chrome->setEnabled(true);
                    explorer->setEnabled(true);
                    wmp->setEnabled(true);
                } else {
                     ui->label_13->setText("Користувач");
                     firefox->setEnabled(false);
                     chrome->setEnabled(false);
                     explorer->setEnabled(false);
                     wmp->setEnabled(false);
                }
                ui->label_17->setText(nickname);
                ui->label_11->setText(QString::number(myID));
                timer1->stop();
                ui->label_19->setText("0:0:0");
            } else if(str.contains("/setuserid")){
                QString resuls = str;
                resuls.replace("/setuserid ", "");
                myID=resuls.toInt();
                ui->pushButton_18->setEnabled(true);
                ui->pushButton_19->setEnabled(true);
                slotSendToServer("/chmessage "+QString::number(myID));
            } else if(str.contains("Authorization failed!")){
                PopUp *popup = new PopUp;
                popup->setPopupText("Ви ввели невірний логін або пароль!");
                popup->show();
            } else if(str.contains("Id no search!")){
                PopUp *popup = new PopUp;
                popup->setPopupText("Користувач з таким ID не знайдений!");
                popup->show();
            } else if(str.contains("Your seclvl is not 2!")){
                PopUp *popup = new PopUp;
                popup->setPopupText("Ви не адміністратор!");
                popup->show();
            } else if(str.contains("/kickreason")){
                kick=true;
                QString resuls = str;
                resuls.replace("/kickreason ", "");
                resuls.replace("_", " ");
                PopUp *popup = new PopUp;
                popup->setPopupText("Вас вигнав адміністратор по причині - "+resuls);
                popup->show();
                logged=false;
                ui->label_4->setText("Оффлайн");
                ui->label_15->setText("Немає");
                ui->label_4->setStyleSheet("color: rgb(0, 0, 0)");
                ui->pushButton_2->setText("Зареєструватися/увійти");
                if(mySecLVL==2){
                    tabs2= ui->tab_2;
                    ui->tabWidget->removeTab(1);
                }
                mySecLVL=0;
                ui->label_11->setText(" ");
                ui->label_17->setText(" ");
                ui->label_13->setText(" ");
                mm=3; sec=0;
                timer1->start();
                timer2->stop();
                timer3->stop();
                ui->pushButton_10->setEnabled(false);
                ui->pushButton_18->setEnabled(false);
                ui->pushButton_19->setEnabled(false);
                firefox->setEnabled(false);
                chrome->setEnabled(false);
                explorer->setEnabled(false);
                wmp->setEnabled(false);
                slotSendToServer("/loggedof "+QString::number(myID)+"\n");
            } else if(str.contains("/firststart")){
                QHostInfo inf;
                slotSendToServer("/addcomputer "+inf.localHostName()+" "+m_pTcpSocket->localAddress().toString()+" "+getMacAddress());

            } else if(str.contains("/getout")){
                QString resuls = str;
                resuls.replace("/getout ", "");
                QTimer::singleShot(resuls.toInt(), this, SLOT(timerupd()));
                PopUp *popup = new PopUp;
                popup->setPopupText("Адміністратор бажає щоб ви вийшли із системи,\n ви будете автоматично виключені через "+resuls+" мілісекунд.");
                popup->show();
            } else if(str=="This computer offline"){
                PopUp *popup = new PopUp;
                popup->setPopupText("Цей комп'ютер не в мережі або не зареєстрований!");
                popup->show();
            } else if(str.contains("/shutdown")){
                QString resuls = str;
                resuls.replace("/shutdown ", "");
                QTimer::singleShot(resuls.toInt(), this, SLOT(timerupd2()));
                PopUp *popup = new PopUp;
                popup->setPopupText("Адміністратор бажає вимкнути систему, ви будете автоматично виключені через "+resuls+" мілісекунд.");
                popup->show();
            } else if(str.contains("/restartcomp")){
                QString resuls = str;
                resuls.replace("/restartcomp ", "");
                QTimer::singleShot(resuls.toInt(), this, SLOT(timerupd3()));
                PopUp *popup = new PopUp;
                popup->setPopupText("Адміністратор бажає перезагрузити систему, ви будете автоматично виключені через "+resuls+" мілісекунд.");
                popup->show();
            } else if(str=="Comp is up"){
                PopUp *popup = new PopUp;
                popup->setPopupText("Вибраний комп'ютер запущений!");
                popup->show();
            } else if(str=="Comp is online"){
                PopUp *popup = new PopUp;
                popup->setPopupText("Вибраний комп'ютер уже був запущений!");
                popup->show();
            } else if(str=="Comp no search"){
                PopUp *popup = new PopUp;
                popup->setPopupText("Вибраний комп'ютер не знайдений у базі!");
                popup->show();
            } else if(str.contains("/setspiscompname")){
                QString resuls = str;
                resuls.replace("/setspiscompname ", "");
                compsname.append(resuls.split(" "));
            } else if(str.contains("/setspisonline")){
                QString resuls = str;
                resuls.replace("/setspisonline ", "");
                compsonline.append(resuls.split(" "));
                dial2 = new QDialog;
                QVBoxLayout *svlay1 = new QVBoxLayout;
                QHBoxLayout *shlay1 = new QHBoxLayout;
                QHBoxLayout *shlay2 = new QHBoxLayout;
                QHBoxLayout *shlay3 = new QHBoxLayout;
                QLabel *label = new QLabel("Список комп'ютерів");
                QLabel *label2 = new QLabel("Ім'я комп'ютера");
                QLabel *label3 = new QLabel("Онлайн");
                lstwid = new QListWidget;
                lstwid2 = new QListWidget;
                QPushButton *pb1 = new QPushButton("Вибрати обраний комп'ютер");
                QPushButton *pb2 = new QPushButton("Закрити");
                connect(pb1, SIGNAL(clicked()), this, SLOT(lstobr()));
                connect(pb2, SIGNAL(clicked()), dial2, SLOT(close()));
                svlay1->addWidget(label);
                shlay1->addWidget(label2);
                shlay1->addWidget(label3);
                svlay1->addLayout(shlay1);
                shlay2->addWidget(lstwid);
                shlay2->addWidget(lstwid2);
                svlay1->addLayout(shlay2);
                shlay3->addWidget(pb1);
                shlay3->addWidget(pb2);
                svlay1->addLayout(shlay3);
                lstwid->addItems(compsname);
                lstwid2->addItems(compsonline);
                connect(lstwid, SIGNAL(currentRowChanged(int)), this, SLOT(rowchangedcspis(int)));
                connect(lstwid2, SIGNAL(currentRowChanged(int)), this, SLOT(rowchangedcspis2(int)));
                dial2->setLayout(svlay1);
                dial2->show();
            } else if(str.contains("/updcomponline")){
                QString resuls = str;
                resuls.replace("/updcomponline ", "");
                ui->label_33->setText(resuls);
            } else if(str.contains("/upduseronline")){
                QString resuls = str;
                resuls.replace("/upduseronline ", "");
                ui->label_37->setText(resuls);
                ui->label_23->setText(resuls);
            } else if(str.contains("/updadminsonline")){
                QString resuls = str;
                resuls.replace("/updadminsonline ", "");
                ui->label_35->setText(resuls);
                ui->label_27->setText(resuls);
            } else if(str.contains("/updalladmins")){
                QString resuls = str;
                resuls.replace("/updalladmins ", "");
                ui->label_39->setText(resuls);
            } else if(str.contains("/updallusers")){
                QString resuls = str;
                resuls.replace("/updallusers ", "");
                ui->label_21->setText(resuls);
            } else if(str.contains("/updlastuser")){
                QString resuls = str;
                resuls.replace("/updlastuser ", "");
                ui->label_25->setText(resuls);
            } else if(str.contains("/setallusersid")){
                QString resuls = str;
                resuls.replace("/setallusersid ", "");
                usersid.append(resuls.split(" "));
            } else if(str.contains("/setallusername")){
                QString resuls = str;
                resuls.replace("/setallusername ", "");
                usersname.append(resuls.split(" "));
            } else if(str.contains("/setalluserlast")){
                QString resuls = str;
                QStringList res2, res3;
                resuls.replace("/setalluserlast ", "");
                res2.append(resuls.split(" "));
                for(int i=0; i<res2.count(); i++){
                    QString test;
                    test.append(res2.at(i));
                    res3.append(test.replace("_", " "));
                }
                userslastenter.append(res3);
            } else if(str.contains("/setallusersyslvl")){
                QString resuls = str;
                resuls.replace("/setallusersyslvl ", "");
                userssyslvl.append(resuls.split(" "));
            } else if(str.contains("/setalluseronline")){
                QString resuls = str;
                resuls.replace("/setalluseronline ", "");
                usersonline.append(resuls.split(" "));
                dial3 = new QDialog;
                QVBoxLayout *usvlay1 = new QVBoxLayout;
                QHBoxLayout *ushlay1 = new QHBoxLayout;
                QHBoxLayout *ushlay2 = new QHBoxLayout;
                QLabel *lab1=new QLabel("Список зареєстрованих користувачів:");
                uslistwid = new QListWidget;
                uslistwid2 = new QListWidget;
                uslistwid3 = new QListWidget;
                uslistwid4 = new QListWidget;
                uslistwid5 = new QListWidget;
                QLabel *lab2 = new QLabel("Ід користувача");
                QLabel *lab3 = new QLabel("Ім'я користувача");
                QLabel *lab4 = new QLabel("Останній вхід користувача");
                QLabel *lab5 = new QLabel("Права користувача(2-адмін)");
                QLabel *lab6 = new QLabel("Онлайн");
                usvlay1->addWidget(lab1);
                ushlay1->addWidget(lab2);
                ushlay1->addWidget(lab3);
                ushlay1->addWidget(lab4);
                ushlay1->addWidget(lab5);
                ushlay1->addWidget(lab6);
                usvlay1->addLayout(ushlay1);
                ushlay2->addWidget(uslistwid);
                ushlay2->addWidget(uslistwid2);
                ushlay2->addWidget(uslistwid3);
                ushlay2->addWidget(uslistwid4);
                ushlay2->addWidget(uslistwid5);
                uslistwid->addItems(usersid);
                uslistwid2->addItems(usersname);
                uslistwid3->addItems(userslastenter);
                uslistwid4->addItems(userssyslvl);
                uslistwid5->addItems(usersonline);
                connect(uslistwid, SIGNAL(currentRowChanged(int)), this, SLOT(rowchangeduspis(int)));
                connect(uslistwid2, SIGNAL(currentRowChanged(int)), this, SLOT(rowchangeduspis2(int)));
                connect(uslistwid3, SIGNAL(currentRowChanged(int)), this, SLOT(rowchangeduspis3(int)));
                connect(uslistwid4, SIGNAL(currentRowChanged(int)), this, SLOT(rowchangeduspis4(int)));
                connect(uslistwid5, SIGNAL(currentRowChanged(int)), this, SLOT(rowchangeduspis5(int)));
                usvlay1->addLayout(ushlay2);
                QPushButton *pb1 = new QPushButton("Закрити");
                connect(pb1, SIGNAL(clicked()), dial3, SLOT(close()));
                usvlay1->addWidget(pb1);
                dial3->setLayout(usvlay1);
                dial3->show();
            } else if(str.contains("/setuserscid")){
                QString resuls = str;
                resuls.replace("/setuserscid ", "");
                userscid.append(resuls.split(" "));
            } else if(str.contains("/setuserscname")){
                QString resuls = str;
                resuls.replace("/setuserscname ", "");
                userscname.append(resuls.split(" "));
            } else if(str.contains("/setusersccname")){
                QString resuls = str;
                resuls.replace("/setusersccname ", "");
                usersccname.append(resuls.split(" "));
            } else if(str.contains("/setuserscip")){
                QString resuls = str;
                resuls.replace("/setuserscip ", "");
                userscip.append(resuls.split(" "));
                dial4 = new QDialog;
                QVBoxLayout *uscvblay = new QVBoxLayout;
                QHBoxLayout *uschblay = new QHBoxLayout;
                QHBoxLayout *uschblay2 = new QHBoxLayout;
                QLabel *lab1 = new QLabel("Список комп'ютерів присутніх користувачів:");
                QLabel *lab2 = new QLabel("Id користувача");
                QLabel *lab3 = new QLabel("Ім'я користувача");
                QLabel *lab4 = new QLabel("Ім'я комп'ютера");
                QLabel *lab5 = new QLabel("Ip комп'ютера");
                uscvblay->addWidget(lab1);
                uschblay->addWidget(lab2);
                uschblay->addWidget(lab3);
                uschblay->addWidget(lab4);
                uschblay->addWidget(lab5);
                uscvblay->addLayout(uschblay);
                usclistwid = new QListWidget;
                usclistwid2 = new QListWidget;
                usclistwid3 = new QListWidget;
                usclistwid4 = new QListWidget;
                connect(usclistwid, SIGNAL(currentRowChanged(int)), this, SLOT(rowchangeduscspis(int)));
                connect(usclistwid2, SIGNAL(currentRowChanged(int)), this, SLOT(rowchangeduscspis2(int)));
                connect(usclistwid3, SIGNAL(currentRowChanged(int)), this, SLOT(rowchangeduscspis3(int)));
                connect(usclistwid4, SIGNAL(currentRowChanged(int)), this, SLOT(rowchangeduscspis4(int)));
                uschblay2->addWidget(usclistwid);
                uschblay2->addWidget(usclistwid2);
                uschblay2->addWidget(usclistwid3);
                uschblay2->addWidget(usclistwid4);
                usclistwid->addItems(userscid);
                usclistwid2->addItems(userscname);
                usclistwid3->addItems(usersccname);
                usclistwid4->addItems(userscip);
                uscvblay->addLayout(uschblay2);
                QPushButton *pb1 = new QPushButton("Закрити");
                connect(pb1, SIGNAL(clicked()), dial4, SLOT(close()));
                uscvblay->addWidget(pb1);
                dial4->setLayout(uscvblay);
                dial4->show();
            } else if(str.contains("/setsusersonline")){
                QString resuls = str;
                resuls.replace("/setsusersonline ", "");
                usersonline.append(resuls.split(" "));
                dial5 = new QDialog;
                QVBoxLayout *vuserslay = new QVBoxLayout;
                QHBoxLayout *huserslay = new QHBoxLayout;
                QHBoxLayout *huserslay2 = new QHBoxLayout;
                QHBoxLayout *huserslay3 = new QHBoxLayout;
                QLabel *lab1 = new QLabel("Список присутніх користувачів:");
                QLabel *lab2 = new QLabel("Id користувача");
                QLabel *lab3 = new QLabel("Ім'я користувача:");
                QLabel *lab4 = new QLabel("Онлайн:");
                vuserslay->addWidget(lab1);
                huserslay->addWidget(lab2);
                huserslay->addWidget(lab3);
                huserslay->addWidget(lab4);
                vuserslay->addLayout(huserslay);
                userlistwid = new QListWidget;
                userlistwid2 = new QListWidget;
                userlistwid5 = new QListWidget;
                connect(userlistwid, SIGNAL(currentRowChanged(int)), this, SLOT(rowchangeduserspis(int)));
                connect(userlistwid2, SIGNAL(currentRowChanged(int)), this, SLOT(rowchangeduserspis2(int)));
                connect(userlistwid5, SIGNAL(currentRowChanged(int)), this, SLOT(rowchangeduserspis3(int)));
                huserslay2->addWidget(userlistwid);
                huserslay2->addWidget(userlistwid2);
                huserslay2->addWidget(userlistwid5);
                vuserslay->addLayout(huserslay2);
                userlistwid->addItems(usersid);
                userlistwid2->addItems(usersname);
                userlistwid5->addItems(usersonline);
                QPushButton *pb1 = new QPushButton("Обрати вибраного користувача");
                QPushButton *pb2 = new QPushButton("Закрити");
                connect(pb1, SIGNAL(clicked()), this, SLOT(slotPb1click()));
                connect(pb2, SIGNAL(clicked()), dial5, SLOT(close()));
                huserslay3->addWidget(pb1);
                huserslay3->addWidget(pb2);
                vuserslay->addLayout(huserslay3);
                dial5->setLayout(vuserslay);
                dial5->show();
            } else if (str.contains("/regencode")){
                QString resuls = str;
                resuls.replace("/regencode ", "");
                QString timed;
                timed.append(resuls);
                int kod=randomBetween(1000000000, 1410065407, static_cast<unsigned int>(QDateTime::currentDateTime().currentMSecsSinceEpoch()));
                slotSendToServer("/checkcodetofree "+QString::number(kod)+" "+timed);
            } else if (str.contains("/codefree")){
                QString resuls = str;
                resuls.replace("/codefree ", "");
                ui->label_47->setText(resuls);
            } else if(str=="/openwritecode"){
                ui->pushButton_10->setEnabled(true);
                mm=3; sec=0;
                timer2 = new QTimer(this);
                connect(timer2, SIGNAL(timeout()), this, SLOT(timerupd6()));
                timer2->start(1000);
                dial6 = new QDialog;
                QVBoxLayout *vlay = new QVBoxLayout;
                QHBoxLayout *hlay = new QHBoxLayout;
                QHBoxLayout *hlay2 = new QHBoxLayout;
                QLabel *lab1 = new QLabel("Введіть 10 значний код:");
                ledcode = new QLineEdit;
                hlay->addWidget(lab1);
                hlay->addWidget(ledcode);
                vlay->addLayout(hlay);
                QPushButton *pb1 = new QPushButton("Відправити");
                QPushButton *pb2 = new QPushButton("Закрити");
                connect(pb2, SIGNAL(clicked()), dial6, SLOT(close()));
                connect(pb1, SIGNAL(clicked()), this, SLOT(codepb1push()));
                hlay2->addWidget(pb1);
                hlay2->addWidget(pb2);
                vlay->addLayout(hlay2);
                dial6->setLayout(vlay);
                dial6->show();
            } else if (str.contains("/codeistrue")){
                QString resuls = str;
                resuls.replace("/codeistrue ", "");
                timer2->stop();
                ui->label_19->setText("0:0:0");
                ui->pushButton_10->setEnabled(false);
                second=resuls.toInt();
                dial6->close();
                timer3 = new QTimer;
                connect(timer3, SIGNAL(timeout()), this, SLOT(timerupd7()));
                timer3->start(1000);
            } else if (str.contains("/1codeistrue1")){
                QString resuls = str;
                resuls.replace("/1codeistrue1 ", "");
                timer2->stop();
                ui->label_19->setText("0:0:0");
                ui->pushButton_10->setEnabled(false);
                second=resuls.toInt();
                timer3 = new QTimer;
                connect(timer3, SIGNAL(timeout()), this, SLOT(timerupd7()));
                timer3->start(1000);
            } else if (str.contains("/updatecodetime")){
                timer3->stop();
                QString resuls = str;
                resuls.replace("/updatecodetime ", "");
                timer2->stop();
                ui->label_19->setText("0:0:0");
                ui->pushButton_10->setEnabled(false);
                second=resuls.toInt();
                timer3 = new QTimer;
                connect(timer3, SIGNAL(timeout()), this, SLOT(timerupd7()));
                timer3->start(1000);
            } else if (str=="/timeadded"){
                PopUp *pop = new PopUp;
                pop->setPopupText("Операцію виконано успішно!");
                pop->show();
            } else if (str.contains("/updlastprogramanduser")){
                QString resuls = str;
                resuls.replace("/updlastprogramanduser ", "");
                QString uid22, pname;
                uid22.append(resuls.split(" ").first());
                pname.append(resuls.split(" ").last());
                ui->label_29->setText(pname);
                ui->label_31->setText(uid22);
            } else if(str=="Account already exists!"){
                PopUp *popup = new PopUp;
                popup->setPopupText("Аккаунт з таким іменем уже існує!");
                popup->show();
            } else if(str=="/regsuccess"){
                slotSendToServer("/login "+ledit->text()+" "+ledit2->text()+"\n");
                nickname = ledit->text();
                dial->close();
            } else if(str=="/codenottrue"){
                PopUp *popup = new PopUp;
                popup->setPopupText("Ваш код уже використаний або не існує!");
                popup->show();
            } else if(str=="Password change"){
                PopUp *popup = new PopUp;
                popup->setPopupText("Пароль успішно змінено!");
                popup->show();
                dial7->close();
            } else if(str=="Change pass incorrect!"){
                PopUp *popup = new PopUp;
                popup->setPopupText("Невірно введений старий пароль!");
                popup->show();
                lineed->setStyleSheet("background-color: rgb(255, 0, 0)");
            } else if(str.contains("/setserverlist")){
                QString resuls = str, nick;
                resuls.replace("/setserverlist ", "");
                nick.append(resuls.split(" ").first());
                //resuls.replace(0, 1, ">");
                resuls.replace(nick+" .", ">");
                ui->textEdit->append("Комп'ютер "+nick+" надсилає команду:\n"+resuls);
            } else if(str=="Admin not online"){
                PopUp *popup = new PopUp;
                popup->setPopupText("Нажаль, всі адміністратори не в мережі!");
                popup->show();
            } else if(str.contains("/messagesended")){
                QString resuls = str;
                resuls.replace("/messagesended ", "");
                if (logged){
                    slotSendToServer("/chmessage "+QString::number(myID));
                }
                PopUp *popup = new PopUp;
                popup->setPopupText("Ваше повідомлення надіслано адміністратору "+resuls.split(" ").first()+", очікуйте відповіді.");
                popup->show();
                dial8->close();
            } else if(str.contains("/setnewadminmes")){
                QString resuls = str;
                resuls.replace("/setnewadminmes ", "");
                QGraphicsColorizeEffect *effect2 = new QGraphicsColorizeEffect(this);
                ui->pushButton_20->setGraphicsEffect(effect2);
                anim2 = new QPropertyAnimation(effect2, "color");
                anim2->setStartValue(QColor(Qt::white));
                anim2->setKeyValueAt(0.5, QColor(Qt::green));
                anim2->setEndValue(QColor(Qt::white));
                anim2->setDuration(2000);
                anim2->setLoopCount(-1);
                countofrow2=resuls.toInt();
                if(resuls.toInt()>0){
                ui->pushButton_20->setEnabled(true);
                ui->pushButton_20->setText("Повідомлення від користувачів ("+resuls+")");
                anim2->start();
                } else {
                   ui->pushButton_20->setEnabled(false);
                   ui->pushButton_20->setText("Повідомлення від користувачів ("+resuls+")");
                   ui->pushButton_20->setGraphicsEffect(ui->pushButton->graphicsEffect());
                   anim2->stop();
                }
            } else if(str.contains("/setnewmesinf")){
                QString resuls = str;
                resuls.replace("/setnewmesinf ", "");
                QString rescount, allcount;
                rescount.append(resuls.split(" ").first());
                allcount.append(resuls.split(" ").last());
                countofrow = allcount.toInt();
                QGraphicsColorizeEffect *effect2 = new QGraphicsColorizeEffect(this);
                ui->pushButton_19->setGraphicsEffect(effect2);
                anim = new QPropertyAnimation(effect2, "color");
                anim->setStartValue(QColor(Qt::white));
                anim->setKeyValueAt(0.5, QColor(Qt::green));
                anim->setEndValue(QColor(Qt::white));
                anim->setDuration(2000);
                anim->setLoopCount(-1);

                if(rescount.toInt()>0){
                    ui->pushButton_19->setText("Мої повідомлення ("+allcount+", є відповіді)");
                    anim->start();
                } else {
                    ui->pushButton_19->setText("Мої повідомлення ("+allcount+")");
                    ui->pushButton_19->setGraphicsEffect(ui->pushButton->graphicsEffect());
                    anim->stop();
                }
            } else if(str.contains("/setusermes")){
                QString resuls = str;
                resuls.replace("/setusermes ", "");
                QString count , mid, act, message, responde;
                count.append(resuls.split(" ").first());
                mid.append(resuls.split(" ").at(1));
                act.append(resuls.split(" ").at(2));
                resuls.replace(count+" "+mid+" "+act+" '", "");
                resuls = resuls.left(resuls.length()-1);
                resuls.replace("' '", "___");
                message.append(resuls.split("___").first());
                responde.append(resuls.split("___").last());
                tblewid->setItem(count.toInt(), 0, new QTableWidgetItem(mid));
                tblewid->setItem(count.toInt(), 1, new QTableWidgetItem(message));
                usrmsgresp.append(responde);
                usrmsg.append(message);
                usrmsgact.append(act);
                usrmid.append(mid);
                if (responde==""){
                    tblewid->setItem(count.toInt(), 2, new QTableWidgetItem("Немає"));
                } else {
                    tblewid->setItem(count.toInt(), 2, new QTableWidgetItem("Є"));
                }
                if (act.toInt()==1 && !(responde=="")){
                    tblewid->item(count.toInt(), 0)->setBackgroundColor(QColor(Qt::green));
                    tblewid->item(count.toInt(), 1)->setBackgroundColor(QColor(Qt::green));
                    tblewid->item(count.toInt(), 2)->setBackgroundColor(QColor(Qt::green));
                } else {
                    tblewid->item(count.toInt(), 0)->setBackgroundColor(QColor(Qt::white));
                    tblewid->item(count.toInt(), 1)->setBackgroundColor(QColor(Qt::white));
                    tblewid->item(count.toInt(), 2)->setBackgroundColor(QColor(Qt::white));
                }
            } else if(str.contains("/setadminmes")){
                QString resuls = str;
                resuls.replace("/setadminmes ", "");
                QString count , mid, uid, message, comp;
                count.append(resuls.split(" ").first());
                mid.append(resuls.split(" ").at(1));
                uid.append(resuls.split(" ").at(2));
                comp.append(resuls.split(" ").at(3));
                resuls.replace(count+" "+mid+" "+uid+" "+comp+" '", "");
                resuls = resuls.left(resuls.length()-1);
                message= resuls;
                tblewid2->setItem(count.toInt(), 0, new QTableWidgetItem(mid));
                tblewid2->setItem(count.toInt(), 1, new QTableWidgetItem(message));
                tblewid2->setItem(count.toInt(), 3, new QTableWidgetItem(comp));
                admmsg.append(message);
                admmsgcomp.append(comp);
                admmsguid.append(uid);
                admmid.append(mid);
                if (uid==""){
                    tblewid2->setItem(count.toInt(), 2, new QTableWidgetItem("Немає"));
                } else {
                    tblewid2->setItem(count.toInt(), 2, new QTableWidgetItem(uid));
                }
            } else if(str=="/newresponse"){
                PopUp *popup = new PopUp;
                popup->setPopupText("У вас нова відповідь від адміністратора!");
                popup->show();
                slotSendToServer("/chmessage "+QString::number(myID));
            } else if(str.contains("/newcompresponse")){
                QString resuls = str;
                resuls.replace("/newcompresponse ", "");
                QString mid, message, response;
                mid.append(resuls.split(" ").first());
                compmid=mid;
                resuls.replace(mid+" '","");
                resuls = resuls.left(resuls.length()-1);
                resuls.replace("' '", "___");
                message.append(resuls.split("___").first());
                response.append(resuls.split("___").last());
                PopUp *popup = new PopUp;
                popup->setPopupText("У вас нова відповідь від адміністратора!");
                popup->show();
                dial13 = new QDialog();
                dial13->setWindowTitle("Повідомлення від адміністратора");
                QVBoxLayout *lay = new QVBoxLayout();
                QHBoxLayout *hlay1 = new QHBoxLayout();
                QLabel *lab1 = new QLabel("Id повідомлення: "+mid);
                QLabel *lab2 = new QLabel("Після закриття цього вікна, ви більше не зможете його побачити!");
                lab2->setStyleSheet(QString("color: rgb(255, 0, 0);"));
                hlay1->addWidget(lab2);
                lay->addLayout(hlay1);
                lay->addWidget(lab1);
                QLabel *lab3 = new QLabel("Повідомлення:");
                lay->addWidget(lab3);
                QHBoxLayout *hlay2 = new QHBoxLayout();
                QTextEdit *textedit1 = new QTextEdit();
                textedit1->setReadOnly(true);
                textedit1->append(message);
                hlay2->addWidget(textedit1);
                lay->addLayout(hlay2);
                QLabel *lab4 = new QLabel("Відповідь:");
                lay->addWidget(lab4);
                QHBoxLayout *hlay3 = new QHBoxLayout();
                QTextEdit *textedit2 = new QTextEdit();
                textedit2->setReadOnly(true);
                textedit2->append(response);
                hlay3->addWidget(textedit2);
                lay->addLayout(hlay3);
                QHBoxLayout *hlay4 = new QHBoxLayout();
                QPushButton *pb1 = new QPushButton("Ok");
                connect(pb1, SIGNAL(clicked()), dial13, SLOT(close()));
                hlay4->addWidget(pb1);
                lay->addLayout(hlay4);
                connect(dial13, SIGNAL(finished(int)), this, SLOT(dial13finish(int)));
                dial13->setLayout(lay);
                dial13->show();
            } else if(str=="You have new mes"){
                PopUp *popup = new PopUp;
                popup->setPopupText("У вас нове повідомлення!");
                popup->show();
            } else if(str.contains("/setkickcount")){
                QString resuls = str;
                resuls.replace("/setkickcount ", "");
                if(resuls.toInt()>0){
                    ui->label_15->setText(resuls);
                } else {
                    ui->label_15->setText("Немає");
                }
            } else if(str.contains("/setenabledprog")){
                QString resuls = str;
                resuls.replace("/setenabledprog ", "");
                if(resuls=="chrome"){
                    chrome->setEnabled(true);
                } else if (resuls=="firefox") {
                    firefox->setEnabled(true);
                } else if (resuls=="winrar") {
                    explorer->setEnabled(true);
                } else if (resuls=="wmp") {
                    wmp->setEnabled(true);
                }
            } else if(str.contains("/setdisabledprog")){
                QString resuls = str;
                resuls.replace("/setdisabledprog ", "");
                if(resuls=="chrome"){
                    chrome->setEnabled(false);
                } else if (resuls=="firefox") {
                    firefox->setEnabled(false);
                } else if (resuls=="winrar") {
                    explorer->setEnabled(false);
                } else if (resuls=="wmp") {
                    wmp->setEnabled(false);
                }
            } else if(str == "Program already banned"){
                PopUp *popup = new PopUp;
                popup->setPopupText("Ця програма уже заблокована у користувача!");
                popup->show();
            } else if(str == "Program dont have"){
                PopUp *popup = new PopUp;
                popup->setPopupText("Ця програма не була заблокована у користувача!");
                popup->show();
            } else if(str.contains("/openthisprog")){
                QString resuls = str;
                resuls.replace("/openthisprog ", "");
                if(resuls=="chrome"){
                    chrome->click();
                } else if (resuls=="firefox") {
                    firefox->click();
                } else if (resuls=="winrar") {
                    explorer->click();
                } else if (resuls=="wmp") {
                    wmp->click();
                }
            } else if(str.contains("/closethisprog")){
                QString resuls = str;
                resuls.replace("/closethisprog ", "");
                if(resuls=="chrome"){
                    QProcess *closep = new QProcess;
                    QStringList arg;
                    arg << "/IM" << "chrome.exe" << "/F";
                    closep->start("Taskkill", arg);
                    closep->waitForFinished();
                    closep->deleteLater();
                } else if (resuls=="firefox") {
                    QProcess *closep = new QProcess;
                    QStringList arg;
                    arg << "/IM" << "firefox.exe" << "/F";
                    closep->start("Taskkill", arg);
                    closep->waitForFinished();
                    closep->deleteLater();
                } else if (resuls=="winrar") {
                    QProcess *closep = new QProcess;
                    QStringList arg;
                    arg << "/IM" << "winrar.exe" << "/F";
                    closep->start("Taskkill", arg);
                    closep->waitForFinished();
                    closep->deleteLater();
                } else if (resuls=="wmp") {
                    QProcess *closep = new QProcess;
                    QStringList arg;
                    arg << "/IM" << "wmplayer.exe" << "/F";
                    closep->start("Taskkill", arg);
                    closep->waitForFinished();
                    closep->deleteLater();
                }
            }

            m_nNextBlockSize = 0;
        }
}
void MyClient::timerupd7(){
    second--;
    ui->label_19->setText(QString::number(SecToH(second))+":"+QString::number(SecToM(second))+":"+QString::number(SecToS(second)));
    if((SecToH(second)==0&&SecToM(second)==5 && SecToS(second)==0)||(SecToH(second)==0&&SecToM(second)==4 && SecToS(second)==0)||(SecToH(second)==0&&SecToM(second)==3 && SecToS(second)==0)||(SecToH(second)==0&&SecToM(second)==2 && SecToS(second)==0)||(SecToH(second)==0&&SecToM(second)==1 && SecToS(second)==0)){
        PopUp *pop = new PopUp;
        pop->setPopupText("Залишилось мало часу!!!!!!!!!");
        pop->show();
    } else if(SecToH(second)==0&&SecToM(second)==0 && SecToS(second)==0){
        QHostInfo inf;
        slotSendToServer("/getout "+inf.localHostName()+" 7000");
        timer3->stop();
    }
}

int MyClient::SecToS(int s){
    int hour, min;
    hour=s/3600;
    min = (s - hour * 3600) / 60;
    return s - hour * 3600 - min * 60;
}
int MyClient::SecToH(int s){
    return s/3600;
}
int MyClient::SecToM(int s){
    int hour;
    hour=s/3600;
    return (s - hour * 3600) / 60;
}
void MyClient::codepb1push(){
    slotSendToServer("/checkontruecode "+ledcode->text());
}
void MyClient::slotPb1click(){
    ui->lineEdit->setText(userlistwid->selectedItems().first()->text());
    dial5->close();
}
void MyClient::rowchangeduserspis(int row){
    userlistwid2->setCurrentRow(row);
    userlistwid5->setCurrentRow(row);
}
void MyClient::rowchangeduserspis2(int row){
    userlistwid->setCurrentRow(row);
    userlistwid5->setCurrentRow(row);
}
void MyClient::rowchangeduserspis3(int row){
    userlistwid->setCurrentRow(row);
    userlistwid2->setCurrentRow(row);
}
void MyClient::rowchangeduscspis(int row){
    usclistwid2->setCurrentRow(row);
    usclistwid3->setCurrentRow(row);
    usclistwid4->setCurrentRow(row);
}
void MyClient::rowchangeduscspis2(int row){
    usclistwid3->setCurrentRow(row);
    usclistwid->setCurrentRow(row);
    usclistwid4->setCurrentRow(row);
}
void MyClient::rowchangeduscspis3(int row){
    usclistwid->setCurrentRow(row);
    usclistwid2->setCurrentRow(row);
    usclistwid4->setCurrentRow(row);
}
void MyClient::rowchangeduscspis4(int row){
    usclistwid->setCurrentRow(row);
    usclistwid2->setCurrentRow(row);
    usclistwid3->setCurrentRow(row);
}
void MyClient::rowchangedcspis(int row){
    lstwid2->setCurrentRow(row);
}
void MyClient::rowchangedcspis2(int row){
    lstwid->setCurrentRow(row);
}
void MyClient::rowchangeduspis(int row){
    uslistwid2->setCurrentRow(row);
    uslistwid3->setCurrentRow(row);
    uslistwid4->setCurrentRow(row);
    uslistwid5->setCurrentRow(row);
}
void MyClient::rowchangeduspis2(int row){
    uslistwid->setCurrentRow(row);
    uslistwid3->setCurrentRow(row);
    uslistwid4->setCurrentRow(row);
    uslistwid5->setCurrentRow(row);
}
void MyClient::rowchangeduspis3(int row){
    uslistwid2->setCurrentRow(row);
    uslistwid->setCurrentRow(row);
    uslistwid4->setCurrentRow(row);
    uslistwid5->setCurrentRow(row);
}
void MyClient::rowchangeduspis4(int row){
    uslistwid2->setCurrentRow(row);
    uslistwid3->setCurrentRow(row);
    uslistwid->setCurrentRow(row);
    uslistwid5->setCurrentRow(row);
}
void MyClient::rowchangeduspis5(int row){
    uslistwid2->setCurrentRow(row);
    uslistwid3->setCurrentRow(row);
    uslistwid4->setCurrentRow(row);
    uslistwid->setCurrentRow(row);
}
void MyClient::timerupd(){
    QProcess* proc=new QProcess(this);
    proc->execute("shutdown /l /f");
}
void MyClient::timerupd2(){
    QProcess* proc=new QProcess(this);
    proc->execute("shutdown /s /f");
}
void MyClient::timerupd3(){
    QProcess* proc=new QProcess(this);
    proc->execute("shutdown /r /f");
}
void MyClient::CloseAllOpenPrograms(){
    QProcess *closep = new QProcess;
    QStringList arg;
    arg << "/IM" << "chrome.exe" << "/IM" << "winrar.exe" << "/IM" << "wmplayer.exe" << "/F";
    closep->start("Taskkill", arg);
    closep->waitForFinished(500);
    closep->deleteLater();
    //system("Taskkill /IM firefox.exe /F");
}
QString MyClient::getMacAddress()
{
    QString text;
    foreach(QNetworkInterface interface, QNetworkInterface::allInterfaces())
    {
        if(interface.type()==QNetworkInterface::Ethernet && interface.flags().testFlag(QNetworkInterface::IsUp)){
        text += interface.hardwareAddress();
        }
    }
    if(text.isEmpty()){
        foreach(QNetworkInterface interface, QNetworkInterface::allInterfaces())
        {
            if(interface.type()==QNetworkInterface::Wifi && interface.flags().testFlag(QNetworkInterface::IsUp)){
            text += interface.hardwareAddress();
            }
        }
    }
    return text;
}

void MyClient::slotError(QAbstractSocket::SocketError err)
{
    QString strError =
        ""+(err == QAbstractSocket::HostNotFoundError ?
                     "The host was not found." :
                     err == QAbstractSocket::RemoteHostClosedError ?
                     "The remote host is closed." :
                     err == QAbstractSocket::ConnectionRefusedError ?
                     "The connection was refused." :
                     QString(m_pTcpSocket->errorString())
                    );
    if(err == QAbstractSocket::RemoteHostClosedError){
        ui->label_2->setText("Відключений");
        ui->label_2->setStyleSheet("color: rgb(255, 0, 0)");
        ui->pushButton->setEnabled(true);
        ui->pushButton_2->setEnabled(false);
        ui->pushButton_2->setText("Зареєструватися/увійти");
        ui->label_4->setText("Оффлайн");
        ui->label_15->setText("Немає");
        ui->label_4->setStyleSheet("color: rgb(0, 0, 0)");
        logged=false;
        if(mySecLVL==2){
            tabs2= ui->tab_2;
            ui->tabWidget->removeTab(1);
        }
        ui->label_11->setText(" ");
        ui->label_17->setText(" ");
        ui->label_13->setText(" ");
        timer1->stop();
        timer3->stop();
        timer2->stop();
        reftimer->stop();
        ui->pushButton_10->setEnabled(false);
        ui->pushButton_14->setEnabled(false);
        ui->pushButton_18->setEnabled(false);
        ui->pushButton_19->setEnabled(false);
        ui->pushButton_17->setEnabled(false);
        ui->pushButton_19->setText("Мої повідомлення (0)");
        anim->stop();
        ui->pushButton_19->setGraphicsEffect(ui->pushButton->graphicsEffect());
        firefox->setEnabled(false);
        chrome->setEnabled(false);
        explorer->setEnabled(false);
        wmp->setEnabled(false);
        CloseAllOpenPrograms();
        ui->label_19->setText("0:0:0");
        mySecLVL=0;
    }
    if (!(err == QAbstractSocket::RemoteHostClosedError && kick==true)){
        PopUp *popup = new PopUp;
        popup->setPopupText("Помилка підключення: "+strError);
        popup->show();
    }
    kick=false;
}

void MyClient::slotSendToServer(QString pov)
{
    QByteArray  arrBlock;
    QDataStream out(&arrBlock, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_4_2);
    out << quint16(0)<< pov;

    out.device()->seek(0);
    out << quint16(static_cast<unsigned int>(arrBlock.size()) - sizeof(quint16));

    m_pTcpSocket->write(arrBlock);
}

void MyClient::slotConnected()
{
    ui->pushButton->setEnabled(false);
    ui->pushButton_2->setEnabled(true);
    ui->label_2->setText("Підключено до серверу");
    QHostInfo inf;
    slotSendToServer("/checkforfirststart "+inf.localHostName());
    timer1 = new QTimer(this);
    connect(timer1, SIGNAL(timeout()), this, SLOT(timerupd5()));
    timer1->start(1000);
    ui->label_2->setStyleSheet("color: rgb(0, 255, 0)");
    ui->pushButton_14->setEnabled(true);
    ui->pushButton_17->setEnabled(true);
    dial = new QDialog();
    dial->setWindowTitle("Система контролю дій користувача");
    QVBoxLayout *vlay = new QVBoxLayout;
    QTabWidget *tabwidgest = new QTabWidget;
    QWidget* tab1 = new QWidget;
    QWidget* tab2 = new QWidget;
    QVBoxLayout *vb = new QVBoxLayout;
    QVBoxLayout *vb2 = new QVBoxLayout;
    QHBoxLayout *hb = new QHBoxLayout;
    QHBoxLayout *hb2 = new QHBoxLayout;
    QLabel *lab1 = new QLabel("Логін: ");
    QLabel *lab3 = new QLabel("Логін: ");
    ledit = new QLineEdit;
    connect(ledit, SIGNAL(textChanged(QString)), this, SLOT(checkSpace(QString)));
    ledit3 = new QLineEdit;
    connect(ledit3, SIGNAL(textChanged(QString)), this, SLOT(checkSpace3(QString)));
    hb->addWidget(lab1);
    hb->addWidget(ledit);
    hb2->addWidget(lab3);
    hb2->addWidget(ledit3);
    QHBoxLayout *hb1 = new QHBoxLayout;
    QHBoxLayout *hb4 = new QHBoxLayout;
    QLabel *lab2 = new QLabel("Пароль: ");
    QLabel *lab4 = new QLabel("Пароль: ");
    ledit2 = new QLineEdit;
    ledit2->setEchoMode(QLineEdit::Password);
    connect(ledit2, SIGNAL(textChanged(QString)), this, SLOT(checkSpace2(QString)));
    ledit4 = new QLineEdit;
    ledit4->setEchoMode(QLineEdit::Password);
    connect(ledit4, SIGNAL(textChanged(QString)), this, SLOT(checkSpace4(QString)));
    hb1->addWidget(lab2);
    hb1->addWidget(ledit2);
    hb4->addWidget(lab4);
    hb4->addWidget(ledit4);
    vb->addLayout(hb);
    vb->addLayout(hb1);
    QPushButton *pb1 = new QPushButton("Зареєструватися");
    connect(pb1, SIGNAL(clicked()), this, SLOT(pb1Clicked()));
    vb->addWidget(pb1);
    vb2->addLayout(hb2);
    vb2->addLayout(hb4);
    QPushButton *pb2 = new QPushButton("Ввійти");
    connect(pb2, SIGNAL(clicked()), this, SLOT(pb2Clicked()));
    vb2->addWidget(pb2);
    tab1->setLayout(vb);
    tab2->setLayout(vb2);
    tabwidgest->addTab(tab1 ,"Реєстрація");
    tabwidgest->addTab(tab2 ,"Вхід");
    vlay->addWidget(tabwidgest);
    dial->setLayout(vlay);
    dial->show();
    QTimer::singleShot(200, this, SLOT(timerupd4()));
    reftimer = new QTimer(this);
        connect(reftimer, SIGNAL(timeout()), this, SLOT(processOneThing()));
        reftimer->start(1000);
}
void MyClient::timerupd5(){
    if (sec==0){
        mm--;
        sec=60;
    }
    if(mm==0 && sec==0){
        QHostInfo inf;
        slotSendToServer("/getout "+inf.localHostName()+" 7000");
        timer1->stop();
    } else if((mm==0 && sec==30) || (mm==1 && sec==0)){
        PopUp *pop = new PopUp;
        pop->setPopupText("Залишилось "+QString::number(mm)+" мінут і "+QString::number(sec)+" секунд!!\n Поспішіть!");
        pop->show();
    }
    ui->label_19->setText("0:"+QString::number(mm)+":"+QString::number(sec));
    sec--;
}
void MyClient::timerupd6(){
    if (sec==0){
        mm--;
        sec=60;
    }
    if(mm==0 && sec==0){
        QHostInfo inf;
        slotSendToServer("/getout "+inf.localHostName()+" 7000");
        timer2->stop();
    } else if((mm==0 && sec==30) || (mm==1 && sec==0)){
        PopUp *pop = new PopUp;
        pop->setPopupText("Залишилось "+QString::number(mm)+" мінут і "+QString::number(sec)+" секунд!!\n Поспішіть!");
        pop->show();
    }
    ui->label_19->setText("0:"+QString::number(mm)+":"+QString::number(sec));
    sec--;
}
void MyClient::processOneThing(){
        if(time==0){
        slotSendToServer("/refresh");
        time=15;
        }
        ui->pushButton_12->setText("Оновити ("+QString::number(time)+")");
        ui->pushButton_14->setText("Оновити ("+QString::number(time)+")");
        time--;
}
void MyClient::timerupd4(){
    slotSendToServer("/refresh");
}
void MyClient::pb1Clicked(){
    if (trues && trues2){
    QString res;
    if(ledit->text().isEmpty() || ledit2->text().isEmpty()){
        PopUp *pop = new PopUp();
        pop->setPopupText("Поля логін або пароль не можуть бути пустими!");
        pop->show();
    } else {
        res.append("/reg "+ledit->text()+" "+ledit2->text()+"\n");
        slotSendToServer(res);
        m_pTcpSocket->waitForBytesWritten();
    }
    } else {
        PopUp *pop = new PopUp();
        pop->setPopupText("В логіні або паролі введені недопустимі символи");
        pop->show();
    }
}
void MyClient::pb2Clicked(){
    if (trues3 && trues4){
    if(ledit3->text().isEmpty() || ledit4->text().isEmpty()){
        PopUp *pop = new PopUp();
        pop->setPopupText("Поля логін або пароль не можуть бути пустими!");
        pop->show();
    } else {
    slotSendToServer("/login "+ledit3->text()+" "+ledit4->text()+"\n");
    nickname = ledit3->text();
    dial->close();
    }
    } else {
        PopUp *pop = new PopUp();
        pop->setPopupText("В логіні або паролі введені недопустимі символи");
        pop->show();
    }
}
void MyClient::checkSpace(QString str){
    if(str.contains(" ") || str.contains("/") || str.contains('?') || str.contains('@') || str.contains("'") || str.contains('\\') || str.contains('"') || str.contains(".") || str.contains(",") || str.contains("#") || str.contains("*") || str.contains("%") || str.contains("!") || str.contains("^") || str.contains("&") || str.contains("(") || str.contains(")") || str.contains("-") || str.contains("+") || str.contains("=") || str.contains("<") || str.contains(">") || str.contains("~") || str.contains("`")){
        ledit->setStyleSheet("background-color: rgb(255, 0, 0)");
        trues=false;
    } else{
        ledit->setStyleSheet("background-color: rgb(255, 255, 255)");
        trues=true;
    }
}
void MyClient::checkSpace2(QString str){
    if(str.contains(" ") || str.contains("/") || str.contains('?') || str.contains('@') || str.contains("'") || str.contains('\\') || str.contains('"') || str.contains(".") || str.contains(",") || str.contains("#") || str.contains("*") || str.contains("%") || str.contains("!") || str.contains("^") || str.contains("&") || str.contains("(") || str.contains(")") || str.contains("-") || str.contains("+") || str.contains("=") || str.contains("<") || str.contains(">") || str.contains("~") || str.contains("`")){
        ledit2->setStyleSheet("background-color: rgb(255, 0, 0)");
        trues2=false;
    } else{
        ledit2->setStyleSheet("background-color: rgb(255, 255, 255)");
        trues2=true;
    }
}
void MyClient::checkSpace3(QString str){
    if(str.contains(" ") || str.contains("/") || str.contains('?') || str.contains('@') || str.contains("'") || str.contains('\\') || str.contains('"') || str.contains(".") || str.contains(",") || str.contains("#") || str.contains("*") || str.contains("%") || str.contains("!") || str.contains("^") || str.contains("&") || str.contains("(") || str.contains(")") || str.contains("-") || str.contains("+") || str.contains("=") || str.contains("<") || str.contains(">") || str.contains("~") || str.contains("`")){
        ledit3->setStyleSheet("background-color: rgb(255, 0, 0)");
        trues3=false;
    } else{
        ledit3->setStyleSheet("background-color: rgb(255, 255, 255)");
        trues3=true;
    }
}
void MyClient::checkSpace4(QString str){
    if(str.contains(" ") || str.contains("/") || str.contains('?') || str.contains('@') || str.contains("'") || str.contains('\\') || str.contains('"') || str.contains(".") || str.contains(",") || str.contains("#") || str.contains("*") || str.contains("%") || str.contains("!") || str.contains("^") || str.contains("&") || str.contains("(") || str.contains(")") || str.contains("-") || str.contains("+") || str.contains("=") || str.contains("<") || str.contains(">") || str.contains("~") || str.contains("`")){
        ledit4->setStyleSheet("background-color: rgb(255, 0, 0)");
        trues4=false;
    } else{
        ledit4->setStyleSheet("background-color: rgb(255, 255, 255)");
        trues4=true;
    }

}
MyClient::~MyClient()
{
    delete ui;
}

void MyClient::on_pushButton_clicked()
{
    m_pTcpSocket->connectToHost(host, port);
}

void MyClient::on_pushButton_2_clicked()
{
    if(logged==false){
        dial->show();
    } else{
        logged=false;
        ui->label_4->setText("Оффлайн");
        ui->label_15->setText("Немає");
        ui->label_4->setStyleSheet("color: rgb(0, 0, 0)");
        ui->pushButton_2->setText("Зареєструватися/увійти");
        if(mySecLVL==2){
            tabs2= ui->tab_2;
            ui->tabWidget->removeTab(1);
        }
        mySecLVL=0;
        ui->label_11->setText(" ");
        ui->label_17->setText(" ");
        ui->label_13->setText(" ");
        mm=3; sec=0;
        timer1->start();
        timer3->stop();
        ui->pushButton_18->setEnabled(false);
        ui->pushButton_19->setEnabled(false);
        chrome->setEnabled(false);
        firefox->setEnabled(false);
        explorer->setEnabled(false);
        wmp->setEnabled(false);
        ui->pushButton_19->setText("Мої повідомлення (0)");
        anim->stop();
        ui->pushButton_19->setGraphicsEffect(ui->pushButton->graphicsEffect());
        timer2->stop();
        CloseAllOpenPrograms();
        slotSendToServer("/loggedof "+QString::number(myID)+"\n");
    }
}

void MyClient::on_pushButton_3_clicked()
{
    if(ui->lineEdit_2->text().contains("/")|| ui->lineEdit->text().contains(" ")){
        PopUp *pop = new PopUp();
        pop->setPopupText("Введені недопустимі символи");
        pop->show();
    }else if(ui->lineEdit->text().contains("ID")){
        PopUp *pop = new PopUp();
        pop->setPopupText("Ви не ввели ід користувача!");
        pop->show();
    }else{
    slotSendToServer("/kick "+ui->lineEdit->text()+" "+ui->lineEdit_2->text().replace(" ", "_")+"\n");
    }
}

void MyClient::on_pushButton_5_clicked()
{
    if(ui->lineEdit_5->text().contains("/")||ui->lineEdit_5->text().contains(" ")){
        PopUp *pop = new PopUp();
        pop->setPopupText("Введені недопустимі символи");
        pop->show();
    } else if(ui->lineEdit_3->text().contains(" ")){
        PopUp *pop = new PopUp();
        pop->setPopupText("Введений недопустимий символ");
        pop->show();
    } else {
    slotSendToServer("/getout "+ui->lineEdit_5->text()+" "+ui->lineEdit_3->text());
    }
}

void MyClient::on_pushButton_6_clicked()
{
    if(ui->lineEdit_5->text().contains("/")||ui->lineEdit_5->text().contains(" ")){
        PopUp *pop = new PopUp();
        pop->setPopupText("Введені недопустимі символи");
        pop->show();
    } else if(ui->lineEdit_3->text().contains(" ")){
        PopUp *pop = new PopUp();
        pop->setPopupText("Введений недопустимий символ");
        pop->show();
    } else {
    slotSendToServer("/shutdown "+ui->lineEdit_5->text()+" "+ui->lineEdit_3->text());
    }
}

void MyClient::on_pushButton_4_clicked()
{
    if(ui->lineEdit_5->text().contains("/")||ui->lineEdit_5->text().contains(" ")){
        PopUp *pop = new PopUp();
        pop->setPopupText("Введені недопустимі символи");
        pop->show();
    } else if(ui->lineEdit_3->text().contains(" ")){
        PopUp *pop = new PopUp();
        pop->setPopupText("Введений недопустимий символ");
        pop->show();
    } else {
    slotSendToServer("/restartcomp "+ui->lineEdit_5->text()+" "+ui->lineEdit_3->text());
    }
}

void MyClient::on_pushButton_7_clicked()
{
    if(ui->lineEdit_5->text().contains("/")||ui->lineEdit_5->text().contains(" ")){
        PopUp *pop = new PopUp();
        pop->setPopupText("Введені недопустимі символи");
        pop->show();
    } else if(ui->lineEdit_3->text().contains(" ")){
        PopUp *pop = new PopUp();
        pop->setPopupText("Введений недопустимий символ");
        pop->show();
    } else {
    slotSendToServer("/wol "+ui->lineEdit_5->text());
    }
}

void MyClient::on_pushButton_8_clicked()
{
    compsname.clear();
    compsonline.clear();
    slotSendToServer("/spiscomp");
}

void MyClient::lstobr(){
    ui->lineEdit_5->setText(lstwid->selectedItems().first()->text());
    dial2->close();
}

void MyClient::on_pushButton_14_clicked()
{
    slotSendToServer("/refresh");
}

void MyClient::on_pushButton_12_clicked()
{
    slotSendToServer("/refresh");
}

void MyClient::on_pushButton_9_clicked()
{
    usersid.clear();
    userslastenter.clear();
    usersname.clear();
    usersonline.clear();
    userssyslvl.clear();
    slotSendToServer("/usersspisall");
}

void MyClient::on_pushButton_11_clicked()
{
    userscid.clear();
    userscname.clear();
    userscip.clear();
    usersccname.clear();
    slotSendToServer("/spisuserscomp");
}

void MyClient::on_pushButton_13_clicked()
{
    usersid.clear();
    usersname.clear();
    usersonline.clear();
    slotSendToServer("/prisuserspis");
}

int MyClient::convertToSec(int h, int m, int s){
    return (h*3600)+(m*60)+s;
}
void MyClient::on_pushButton_15_clicked()
{
    /*QDateTime startdate;
    startdate= QDateTime::currentDateTime();
    QDateTime enddate;
    enddate= startdate.addSecs(convertToSec(ui->lineEdit_4->text().toInt(), ui->lineEdit_6->text().toInt(), ui->lineEdit_7->text().toInt()));*/
    if(ui->lineEdit_4->text().contains(" ")||ui->lineEdit_6->text().contains(" ")||ui->lineEdit_7->text().contains(" ")){
        PopUp *pop = new PopUp();
        pop->setPopupText("Введені недопустимі символи!");
        pop->show();
    } else {
    int kod=randomBetween(1000000000, 1410065407, static_cast<unsigned int>(QDateTime::currentDateTime().currentMSecsSinceEpoch()));
    slotSendToServer("/checkcodetofree "+QString::number(kod)+" "+QString::number(convertToSec(ui->lineEdit_4->text().toInt(), ui->lineEdit_6->text().toInt(), ui->lineEdit_7->text().toInt())));
    }
}

void MyClient::on_pushButton_10_clicked()
{
    dial6->show();
}

void MyClient::on_pushButton_16_clicked()
{
    if(ui->lineEdit_4->text().contains(" ")||ui->lineEdit_6->text().contains(" ")||ui->lineEdit_7->text().contains(" ")||ui->lineEdit_8->text().contains(" ")){
        PopUp *pop = new PopUp();
        pop->setPopupText("Введені недопустимі символи!");
        pop->show();
    } else {
     slotSendToServer("/addtimetouser "+ui->lineEdit_8->text()+" "+QString::number(convertToSec(ui->lineEdit_4->text().toInt(), ui->lineEdit_6->text().toInt(), ui->lineEdit_7->text().toInt())));
    }
}

void MyClient::on_pushButton_18_clicked()
{
     dial7 = new QDialog();
     dial7->setWindowTitle("Зміна паролю аккаунта "+ nickname);
     QVBoxLayout *lay = new QVBoxLayout();
     QLabel *label1 = new QLabel("Старий пароль: ");
     lineed = new QLineEdit();
     connect(lineed, SIGNAL(textChanged(QString)), this, SLOT(checkSpace5(QString)));
     lineed->setEchoMode(QLineEdit::Password);
     QHBoxLayout *hlay = new QHBoxLayout();
     hlay->addWidget(label1);
     hlay->addWidget(lineed);
     lay->addLayout(hlay);
     QLabel *label2 = new QLabel("Новий пароль: ");
     lineed2 = new QLineEdit();
     connect(lineed2, SIGNAL(textChanged(QString)), this, SLOT(checkSpace6(QString)));
     lineed2->setEchoMode(QLineEdit::Password);
     QHBoxLayout *hlay2 = new QHBoxLayout();
     hlay2->addWidget(label2);
     hlay2->addWidget(lineed2);
     lay->addLayout(hlay2);
     QLabel *label3 = new QLabel("Підтвердіть пароль: ");
     lineed3 = new QLineEdit();
     connect(lineed3, SIGNAL(textChanged(QString)), this, SLOT(checkSpace7(QString)));
     lineed3->setEchoMode(QLineEdit::Password);
     QHBoxLayout *hlay3 = new QHBoxLayout();
     hlay3->addWidget(label3);
     hlay3->addWidget(lineed3);
     lay->addLayout(hlay3);
     QHBoxLayout *hlay4 = new QHBoxLayout();
     QPushButton *pb1 = new QPushButton("Змінити пароль");
     QPushButton *pb2 = new QPushButton("Вийти");
     connect(pb1, SIGNAL(clicked()), this, SLOT(pb3Clicked()));
     connect(pb2, SIGNAL(clicked()), dial7, SLOT(close()));
     hlay4->addWidget(pb1);
     hlay4->addWidget(pb2);
     lay->addLayout(hlay4);
     dial7->setLayout(lay);
     dial7->show();
}

void MyClient::pb3Clicked(){
    if (trues5 && trues6 && trues7){
    if(lineed->text().isEmpty() || lineed2->text().isEmpty() || lineed3->text().isEmpty()){
        PopUp *pop = new PopUp();
        pop->setPopupText("Поля не можуть бути пустими!");
        pop->show();
    } else if (!(lineed2->text()==lineed3->text())){
        PopUp *pop = new PopUp();
        pop->setPopupText("Новий пароль не співпадає з його підтвердженням!");
        pop->show();
    } else {
        slotSendToServer("/changepass "+QString::number(myID)+" "+lineed->text()+" "+lineed2->text());
    }
    } else {
        PopUp *pop = new PopUp();
        pop->setPopupText("Введені недопустимі символи");
        pop->show();
    }
}

void MyClient::checkSpace5(QString str){
    if(str.contains(" ") || str.contains("/") || str.contains('?') || str.contains('@') || str.contains("'") || str.contains('\\') || str.contains('"') || str.contains(".") || str.contains(",") || str.contains("#") || str.contains("*") || str.contains("%") || str.contains("!") || str.contains("^") || str.contains("&") || str.contains("(") || str.contains(")") || str.contains("-") || str.contains("+") || str.contains("=") || str.contains("<") || str.contains(">") || str.contains("~") || str.contains("`")){
        lineed->setStyleSheet("background-color: rgb(255, 0, 0)");
        trues5=false;
    } else{
        lineed->setStyleSheet("background-color: rgb(255, 255, 255)");
        trues5=true;
    }
}

void MyClient::checkSpace6(QString str){
    if(str.contains(" ") || str.contains("/") || str.contains('?') || str.contains('@') || str.contains("'") || str.contains('\\') || str.contains('"') || str.contains(".") || str.contains(",") || str.contains("#") || str.contains("*") || str.contains("%") || str.contains("!") || str.contains("^") || str.contains("&") || str.contains("(") || str.contains(")") || str.contains("-") || str.contains("+") || str.contains("=") || str.contains("<") || str.contains(">") || str.contains("~") || str.contains("`")){
        lineed2->setStyleSheet("background-color: rgb(255, 0, 0)");
        trues6=false;
    } else{
        lineed2->setStyleSheet("background-color: rgb(255, 255, 255)");
        trues6=true;
    }
}

void MyClient::checkSpace7(QString str){
    if(str.contains(" ") || str.contains("/") || str.contains('?') || str.contains('@') || str.contains("'") || str.contains('\\') || str.contains('"') || str.contains(".") || str.contains(",") || str.contains("#") || str.contains("*") || str.contains("%") || str.contains("!") || str.contains("^") || str.contains("&") || str.contains("(") || str.contains(")") || str.contains("-") || str.contains("+") || str.contains("=") || str.contains("<") || str.contains(">") || str.contains("~") || str.contains("`")){
        lineed3->setStyleSheet("background-color: rgb(255, 0, 0)");
        trues7=false;
    } else{
        lineed3->setStyleSheet("background-color: rgb(255, 255, 255)");
        trues7=true;
    }
}

void MyClient::on_pushButton_17_clicked()
{
    dial8 = new QDialog;
    dial8->setWindowTitle("Повідомлення адміністратору");
    QVBoxLayout *lay = new QVBoxLayout();
    QHBoxLayout *hlay= new QHBoxLayout();
    QLabel *label1 = new QLabel("Повідомлення: ");
    tedit = new QTextEdit();
    hlay->addWidget(label1);
    hlay->addWidget(tedit);
    lay->addLayout(hlay);
    chb = new QCheckBox("Приорітет на присутніх адміністраторів");
    lay->addWidget(chb);
    QHBoxLayout *hlay2 = new QHBoxLayout();
    QPushButton *pb1 = new QPushButton("Відправити");
    QPushButton *pb2 = new QPushButton("Закрити");
    connect(pb1, SIGNAL(clicked()), this, SLOT(pb4Clicked()));
    connect(pb2, SIGNAL(clicked()), dial8, SLOT(close()));
    hlay2->addWidget(pb1);
    hlay2->addWidget(pb2);
    lay->addLayout(hlay2);
    dial8->setLayout(lay);
    dial8->show();
}

void MyClient::pb4Clicked(){
    if(tedit->toPlainText().contains("/")){
        PopUp *pop = new PopUp();
        pop->setPopupText("Введені недопустимі символи!");
        pop->show();
    }else {
    if(chb->isChecked()){
        if(logged){
            slotSendToServer("/1addadminmesslogged "+QString::number(myID)+" '"+tedit->toPlainText()+"'");
        } else {
            slotSendToServer("/1addmesstoadmin '"+tedit->toPlainText()+"'");
        }
    } else{
        if(logged){
            slotSendToServer("/addadminmesslogged "+QString::number(myID)+" '"+tedit->toPlainText()+"'");
        } else {
            slotSendToServer("/addmesstoadmin '"+tedit->toPlainText()+"'");
        }
    }
    }
}

void MyClient::on_pushButton_19_clicked()
{
    dial9 = new QDialog();
    dial9->setWindowTitle("Мої повідомлення");
    dial9->setFixedWidth(375);
    QVBoxLayout *lay = new QVBoxLayout;
    gllab = new QLabel("Cторінка 1 з 1");
    lay->addWidget(gllab);
    tblewid = new QTableWidget(countofrow, 3,this);
    QStringList list;
    list << "ID повідомлення" << "Текст повідомлення" << "Відповідь";
    tblewid->setHorizontalHeaderLabels(list);
    tblewid->setSelectionMode(QAbstractItemView::SingleSelection);
    tblewid->setSelectionBehavior(QAbstractItemView::SelectRows);
    tblewid->setEditTriggers(QAbstractItemView::NoEditTriggers);
    tblewid->setFixedWidth(365);
    tblewid->setColumnWidth(1, 121);
    tblewid->selectRow(0);
    lay->addWidget(tblewid);
    QHBoxLayout *hboxl1 = new QHBoxLayout();
    QPushButton *pb1 = new QPushButton("Обрати");
    QPushButton *pb2 = new QPushButton("Закрити");
    connect(pb1, SIGNAL(clicked()), this, SLOT(pb5Clicked()));
    connect(pb2, SIGNAL(clicked()), dial9, SLOT(close()));
    hboxl1->addWidget(pb1);
    hboxl1->addWidget(pb2);
    lay->addLayout(hboxl1);
    usrmsg.clear();
    usrmsgresp.clear();
    usrmsgact.clear();
    usrmid.clear();
    slotSendToServer("/getusermes "+QString::number(myID));
    dial9->setLayout(lay);
    dial9->show();
}

void MyClient::pb5Clicked(){
    dial9->close();
    idnomer=tblewid->selectedItems().first()->row();
    dial10 = new QDialog();
    dial10->setWindowTitle("Перегляд повідомлення "+QString::number(idnomer));
    connect(dial10, SIGNAL(finished(int)), this, SLOT(dial10finish(int)));
    QVBoxLayout *lay = new QVBoxLayout();
    QHBoxLayout *hlay1 = new QHBoxLayout();
    QLabel *lab1 = new QLabel("Номер повідомлення:    "+QString::number(idnomer));
    QLabel *lab2 = new QLabel();
    hlay1->addWidget(lab1);
    hlay1->addWidget(lab2);
    lay->addLayout(hlay1);
    QHBoxLayout *hlay2 = new QHBoxLayout();
    QLabel *lab3 = new QLabel("Повідомлення: ");
    QTextEdit *reste = new QTextEdit();
    reste->append(usrmsg.at(idnomer));
    reste->setReadOnly(true);
    hlay2->addWidget(lab3);
    hlay2->addWidget(reste);
    lay->addLayout(hlay2);
    QHBoxLayout *hlay3 = new QHBoxLayout();
    QLabel *lab4 = new QLabel("Відповідь:   ");
    QTextEdit *reste2 = new QTextEdit();
    reste2->append(usrmsgresp.at(idnomer));
    reste2->setReadOnly(true);
    hlay3->addWidget(lab4);
    hlay3->addWidget(reste2);
    lay->addLayout(hlay3);
    QHBoxLayout *hlay4 = new QHBoxLayout();
    QPushButton *pb1 = new QPushButton("Ок");
    connect(pb1, SIGNAL(clicked()), dial10, SLOT(close()));
    hlay4->addWidget(pb1);
    lay->addLayout(hlay4);
    dial10->setLayout(lay);
    dial10->show();
}

void MyClient::dial10finish(int kod){
    if(!(usrmsgresp.at(idnomer)=="")){
        slotSendToServer("/setseemsg "+usrmid.at(idnomer)+" "+QString::number(myID));
    }
    slotSendToServer("/chmessage "+QString::number(myID));
    Q_UNUSED(kod);
}

void MyClient::on_pushButton_20_clicked()
{
    dial11 = new QDialog();
    dial11->setWindowTitle("Нові повідомлення");
    dial11->setFixedWidth(495);
    QVBoxLayout *lay = new QVBoxLayout;
    tblewid2 = new QTableWidget(countofrow2, 4,this);
    QStringList list;
    list << "ID повідомлення" << "Текст повідомлення" << "Відправник(id)" << "Комп'ютер";
    tblewid2->setHorizontalHeaderLabels(list);
    tblewid2->setSelectionMode(QAbstractItemView::SingleSelection);
    tblewid2->setSelectionBehavior(QAbstractItemView::SelectRows);
    tblewid2->setEditTriggers(QAbstractItemView::NoEditTriggers);
    tblewid2->setFixedWidth(485);
    tblewid2->setColumnWidth(0, 122);
    tblewid2->setColumnWidth(1, 122);
    tblewid2->selectRow(0);
    lay->addWidget(tblewid2);
    QHBoxLayout *hboxl1 = new QHBoxLayout();
    QPushButton *pb1 = new QPushButton("Обрати");
    QPushButton *pb2 = new QPushButton("Закрити");
    connect(pb1, SIGNAL(clicked()), this, SLOT(pb6Clicked()));
    connect(pb2, SIGNAL(clicked()), dial11, SLOT(close()));
    hboxl1->addWidget(pb1);
    hboxl1->addWidget(pb2);
    lay->addLayout(hboxl1);
    admmsg.clear();
    admmsgcomp.clear();
    admmsguid.clear();
    admmid.clear();
    slotSendToServer("/getadminmes "+QString::number(myID));
    dial11->setLayout(lay);
    dial11->show();
}

void MyClient::pb6Clicked(){
    dial11->close();
    idnomer2=tblewid2->selectedItems().first()->row();
    dial12 = new QDialog();
    dial12->setWindowTitle("Перегляд повідомлення "+QString::number(idnomer2));
    connect(dial12, SIGNAL(finished(int)), this, SLOT(dial12finish(int)));
    QVBoxLayout *lay = new QVBoxLayout();
    QHBoxLayout *hlay1 = new QHBoxLayout();
    QLabel *lab1 = new QLabel("Id повідомлення:    "+admmid.at(idnomer2));
    QLabel *lab2 = new QLabel();
    hlay1->addWidget(lab1);
    hlay1->addWidget(lab2);
    lay->addLayout(hlay1);
    QHBoxLayout *hlay2 = new QHBoxLayout();
    QLabel *lab3 = new QLabel("Повідомлення: ");
    QTextEdit *reste = new QTextEdit();
    reste->append(admmsg.at(idnomer2));
    reste->setReadOnly(true);
    hlay2->addWidget(lab3);
    hlay2->addWidget(reste);
    lay->addLayout(hlay2);
    QHBoxLayout *hlay3 = new QHBoxLayout();
    QLabel *lab4 = new QLabel("Відповідь:    ");
    admreste = new QTextEdit();
    hlay3->addWidget(lab4);
    hlay3->addWidget(admreste);
    lay->addLayout(hlay3);
    QHBoxLayout *hlay4 = new QHBoxLayout();
    QPushButton *pb1 = new QPushButton("Відправити");
    connect(pb1, SIGNAL(clicked()), this, SLOT(admresteclck()));
    hlay4->addWidget(pb1);
    lay->addLayout(hlay4);
    dial12->setLayout(lay);
    dial12->show();
}
void MyClient::admresteclck(){
    if(admreste->toPlainText().contains("/")){
        PopUp *pop = new PopUp();
        pop->setPopupText("Введені недопустимі символи!");
        pop->show();
    } else {
        dial12->close();
    }
}

void MyClient::dial12finish(int kod){
    if(admreste->toPlainText().isEmpty()|| admreste->toPlainText().contains("/")){
        PopUp *pop = new PopUp();
        pop->setPopupText("При повторному перегляді надайте відповідь, будь-ласка!");
        pop->show();
    } else {
    if(!(admmsguid.at(idnomer2)=="")){
        slotSendToServer("/setresponsemsg "+admmid.at(idnomer2)+" "+admmsguid.at(idnomer2)+ " '" +admreste->toPlainText().replace("\n", "@enter@")+"'");
    } else {
        slotSendToServer("/setcompresponsemsg "+admmid.at(idnomer2)+" "+admmsgcomp.at(idnomer2)+" '"+admreste->toPlainText().replace("\n", "@enter@")+"'");
    }
    slotSendToServer("/chmessage "+QString::number(myID));
    }
    Q_UNUSED(kod);
}

void MyClient::dial13finish(int kod){
    slotSendToServer("/setcompsee "+compmid);
    Q_UNUSED(kod);
}

void MyClient::on_pushButton_21_clicked()
{
    if(ui->lineEdit_9->text().contains(" ")){
        PopUp *pop = new PopUp();
        pop->setPopupText("Введений недопустимий символ!");
        pop->show();
    } else if(ui->lineEdit_9->text().contains("User ID")){
        PopUp *pop = new PopUp();
        pop->setPopupText("Введіть ід користувача!");
        pop->show();
    } else {
    slotSendToServer("/disableprogram "+ui->lineEdit_9->text()+" "+ui->comboBox->currentText());
    }
}

void MyClient::on_pushButton_22_clicked()
{
    if(ui->lineEdit_9->text().contains(" ")){
        PopUp *pop = new PopUp();
        pop->setPopupText("Введений недопустимий символ!");
        pop->show();
    } else {
    slotSendToServer("/enableprogram "+ui->lineEdit_9->text()+" "+ui->comboBox->currentText());
    }
}

void MyClient::on_pushButton_23_clicked()
{
    if(ui->lineEdit_9->text().contains(" ")){
        PopUp *pop = new PopUp();
        pop->setPopupText("Введений недопустимий символ!");
        pop->show();
    } else {
    slotSendToServer("/openprogram "+ui->lineEdit_9->text()+" "+ui->comboBox->currentText());
    }
}

void MyClient::on_pushButton_24_clicked()
{
    if(ui->lineEdit_9->text().contains(" ")){
        PopUp *pop = new PopUp();
        pop->setPopupText("Введений недопустимий символ!");
        pop->show();
    } else {
    slotSendToServer("/closeprogram "+ui->lineEdit_9->text()+" "+ui->comboBox->currentText());
    }
}
