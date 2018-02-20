#include "network.h"

bool Network::isPowerOfTwo(int value)
{
    return (value & (value - 1)) == 0;
}

double Network::log4(double value)
{
    return log(value)/log(4);
}

Network::Network(QDataStream &s)
{
    s >> inputCount;
    s >> width;
    s >> height;

    unsigned int epochCount;
    unsigned int x,y;
    double lastError;
    s >> epochCount;
    s >> x;
    s >> y;
    s >> lastError;

    lastN = new NeuronNi(height);

    for(unsigned int i = 0; i < height; i++){
        s >> lastN->weights[i];
    }
    s >> lastN->bias;

    layers = new NeuronNi**[width];
    resultsFromLayers = new double*[width];

    for(unsigned int layerIndex = 0; layerIndex < width-1; layerIndex++){
        resultsFromLayers[layerIndex] = new double[height];
        layers[layerIndex] = new NeuronNi*[height];
        for(unsigned int i = 0; i < height; i++){
            layers[layerIndex][i] = new NeuronNi(height);

            for(unsigned int j = 0; j < layers[layerIndex][i]->inputCount; j++){
                s >> layers[layerIndex][i]->weights[j];
            }
            s >> layers[layerIndex][i]->bias;

        }
    }

    resultsFromLayers[width-1] = new double[height];

    layers[width-1] = new NeuronNi*[height];
    for(unsigned int i = 0; i < height; i++){
        layers[width-1][i] = new NeuronNi(inputCount);
        for(unsigned int j = 0; j < layers[width-1][i]->inputCount; j++){
            s >> layers[width-1][i]->weights[j];
        }
        s >> layers[width-1][i]->bias;
    }
}

Network::~Network()
{
    delete lastN;

    for(unsigned int layerIndex = 0; layerIndex < width; layerIndex++){
        for(unsigned int i = 0; i < height; i++){
            delete layers[layerIndex][i];
        }
        delete[] layers[layerIndex];
        delete[] resultsFromLayers[layerIndex];
    }
    delete[] layers;
    delete[] resultsFromLayers;


}

double Network::work(const double *inputs, unsigned int size)
{
    double result = -1;
    if(inputCount == size){

        for(unsigned int i = 0; i < height;i++){
            resultsFromLayers[width-1][i] = layers[width-1][i]->work(inputs,size);
        }
        for(int layerIndex = width-2; layerIndex >= 0; layerIndex--){
            for(unsigned int i = 0; i < height; i++){
                resultsFromLayers[layerIndex][i] = layers[layerIndex][i]->work(resultsFromLayers[layerIndex+1],height);
            }
        }
        result = lastN->work(resultsFromLayers[0],height);
        lastResult = result;
    }
    return result;
}

Network::Network()
{
    this->inputCount = 0;
    this->width = 0;
    this->height = 0;
}
