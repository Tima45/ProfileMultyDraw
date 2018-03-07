#ifndef NEURONNI_H
#define NEURONNI_H

#include "neuron.h"
#include <QDataStream>
#include <QDebug>

class NeuronNi : public Neuron
{
public:
    explicit NeuronNi(unsigned int inputCount);
    NeuronNi(const NeuronNi &other);
    ~NeuronNi();
    double work(const double *inputs,unsigned int inputCount);
    double calculateCorrection(double speed,double error,const double *inputs,unsigned int inputCount);
    void updateWeights();
    void save(QDataStream &s);
    void load(QDataStream &s);

    int getInputCount();
private:
    unsigned int inputCount;
    double *weights;
    double *deltaWeights;
    double bias = 0;
    double deltaBias = 0;
};

#endif // NEURONNI_H
