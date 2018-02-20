#include "profilegenerator.h"

ProfileGenerator::ProfileGenerator(QString loadPath, QObject *parent) : QObject(parent)
{
    QFile f(loadPath);
    if(f.open(QIODevice::ReadOnly)){
        QDataStream s(&f);

        s >> profileSize;
        for(unsigned int y = 0; y < profileSize; y++){
            networks.append(QVector<Network*>());
            for(unsigned int x = 0; x < profileSize; x++){
                Network *n = new Network(s);
                networks.last().append(n);
            }
        }

        f.close();
    }else{
        qDebug() << "wrong file";
    }
}
ProfileGenerator::~ProfileGenerator(){
    for(unsigned int y = 0; y < profileSize; y++){
        for(unsigned int x = 0; x < profileSize; x++){
            delete networks[y][x];
        }
    }
}

void ProfileGenerator::generate(QVector<QVector<double> > &result, QVector<double> xyProfile)
{
    if(profileSize*2 == xyProfile.count()){
        for(unsigned int y = 0; y < profileSize; y++){
            for(unsigned int x = 0; x < profileSize; x++){
                result[y][x] = networks.at(y).at(x)->work(xyProfile.constData(),xyProfile.count());
                //qDebug() << y << x;
            }
        }
    }else{
        qDebug() << "profileSize == xyProfile.count()";
    }
}
