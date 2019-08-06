#ifndef FILESETTINGS_H
#define FILESETTINGS_H

#include <QObject>
#include <QFile>
#include <QTextStream>

class FileSettings
{
public:
    explicit FileSettings(QString property);
    QStringList getAllProperty();
    QString getProperty(QString pname);
    bool isNotFile();
private:
    QStringList proplst;
    bool filenotopen=false;
};

#endif // FILESETTINGS_H
