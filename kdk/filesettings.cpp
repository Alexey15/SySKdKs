#include "filesettings.h"

FileSettings::FileSettings(QString property)
{
    QFile file("settings.txt");
        if (!file.open(QIODevice::ReadOnly | QIODevice::Text)){
           filenotopen=true;
           return;
        }
        bool hasprop=false;
        QTextStream in(&file);
        QString line = in.readLine();
        while (!line.isNull()) {
           if (line == property+":"){
               hasprop=true;
           } else if (line == "."){
               hasprop=false;
           }
           if(hasprop){
               proplst.append(line);
           }
           line = in.readLine();
        }
}

QStringList FileSettings::getAllProperty(){
    return proplst;
}

QString FileSettings::getProperty(QString pname){
    foreach(QString name, proplst){
        if(name.contains(pname)){
            name.replace("\t","");
            name.replace(" ", "");
            name.replace(":", " ");
            return name.split(" ").last();
        }
    }
    return nullptr;
}

bool FileSettings::isNotFile(){
    return filenotopen;
}
