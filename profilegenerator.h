#ifndef PROFILEGENERATOR_H
#define PROFILEGENERATOR_H

#include <QObject>
#include <QDebug>
#include <QFile>
#include "network.h"

class ProfileGenerator : public QObject
{
    Q_OBJECT
public:
    ProfileGenerator(QString loadPath,QObject *parent = 0);
    ~ProfileGenerator();
    void generate(QVector<QVector<double>> &result,QVector<double> xyProfile);
    QVector<QVector<Network*>> networks;
    unsigned int profileSize = 0;


signals:

public slots:
};

#endif // PROFILEGENERATOR_H
