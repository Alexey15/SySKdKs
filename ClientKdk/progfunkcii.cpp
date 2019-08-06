#include "progfunkcii.h"

progfunkcii::progfunkcii(QString programname, QString ProgDir, QStringList arguments, QWidget *parent) : QPushButton(parent)
{
    setText(programname);
    ProgD=ProgDir;
    arg=arguments;
    connect(this, SIGNAL(clicked()), this, SLOT(ButtClick()));
}

void progfunkcii::ButtClick(){
    QProcess *proc = new QProcess(this);
    proc->waitForFinished(1000);
    proc->start(ProgD, arg);
    connect(proc, SIGNAL(finished(int)), this, SLOT(ProgramClose(int)));
    emit ProgramOpened();
}

void progfunkcii::setSS(const QString &styleSheet){
    setStyleSheet(styleSheet);
}

void progfunkcii::ProgramClose(int excode){
    emit ProgramClosed();
    Q_UNUSED(excode);
}
void progfunkcii::ExplorerClose(){
    emit ProgramClosed();
}
