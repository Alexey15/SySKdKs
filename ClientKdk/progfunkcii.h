#ifndef PROGFUNKCII_H
#define PROGFUNKCII_H

#include <QObject>
#include <QPushButton>
#include <QString>
#include <QProcess>

class progfunkcii : public QPushButton
{
    Q_OBJECT
public:
    explicit progfunkcii(QString programname, QString ProgDir, QStringList arguments, QWidget *parent = nullptr);
    void setSS(const QString &styleSheet);
private:
   QString ProgD;
   QStringList arg;
signals:
   void ProgramClosed();
   void ProgramOpened();
private slots:
    void ButtClick();
    void ProgramClose(int excode);
    void ExplorerClose();
};

#endif // PROGFUNKCII_H
