#ifndef NETWORK_H
#define NETWORK_H

#include <math.h>
#include <QDataStream>
#include "neuron2i.h"
#include "neuronni.h"


class Network
{
public:
    static bool isPowerOfTwo(int value);
    static double log4(double value);
    explicit Network(QDataStream &s);
    virtual ~Network();
    double work(const double *inputs,unsigned int size);

    NeuronNi ***layers;
    NeuronNi *lastN;
    double **resultsFromLayers;


    unsigned int inputCount = 0;
    unsigned int width = 0;
    unsigned int height = 0;
    Network();
    double lastResult = 0;

};

#endif // NETWORK_H
