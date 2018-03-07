#ifndef NETWORK_H
#define NETWORK_H

#include <math.h>
#include <QDataStream>
#include <QDebug>
#include "neuronni.h"
#include "networkworkplace.h"


class Network
{
public:
    Network(unsigned int inputCount,unsigned int width, unsigned int height);
    Network(const Network &other);
    virtual ~Network();

    void load(QDataStream &s);
    void save(QDataStream &s);
    double work(const double *inputs,unsigned int size, NetworkWorkPlace *workPlace);

    int getWidth();
    int getHeight();
    int getInputCount();
protected:
    NeuronNi ***layers;
    NeuronNi *lastN;

    unsigned int inputCount = 0;
    unsigned int width = 0;
    unsigned int height = 0;

    int networkType = 0;
};

#endif // NETWORK_H
