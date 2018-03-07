#include "network.h"

Network::Network(unsigned int inputCount, unsigned int width, unsigned int height)
{
    this->inputCount = inputCount;
    this->width = width;
    this->height = height;

    lastN = new NeuronNi(height);

    layers = new NeuronNi**[width];

    for(unsigned int layerIndex = 0; layerIndex < width-1; layerIndex++){
        layers[layerIndex] = new NeuronNi*[height];
        for(unsigned int i = 0; i < height; i++){
            layers[layerIndex][i] = new NeuronNi(height);
        }
    }

    layers[width-1] = new NeuronNi*[height];
    for(unsigned int i = 0; i < height; i++){
        layers[width-1][i] = new NeuronNi(inputCount);
    }
}

Network::Network(const Network &other)
{
    inputCount = other.inputCount;
    width = other.width;
    height = other.height;

    lastN = new NeuronNi(*other.lastN);

    layers = new NeuronNi**[width];

    for(unsigned int layerIndex = 0; layerIndex < width; layerIndex++){
        layers[layerIndex] = new NeuronNi*[height];
        for(unsigned int i = 0; i < height; i++){
            layers[layerIndex][i] = new NeuronNi(*other.layers[layerIndex][i]);
        }
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
    }
    delete[] layers;
}

void Network::load(QDataStream &s)
{
    unsigned int _inputCount = 0;
    unsigned int _width = 0;
    unsigned int _height = 0;
    int _networkType = 0;
    s >> _networkType;
    s >> _inputCount;
    s >> _width;
    s >> _height;
    if(_width != width || _height != height || _inputCount != inputCount || _networkType != networkType){
        qDebug() << "Network error while loading";
        throw;
    }else{
        lastN->load(s);
        for(unsigned int layerIndex = 0; layerIndex < width; layerIndex++){
            for(unsigned int i = 0; i < height; i++){
                layers[layerIndex][i]->load(s);
            }
        }
    }
}

void Network::save(QDataStream &s)
{
    s << networkType;
    s << inputCount;
    s << width;
    s << height;
    lastN->save(s);
    for(unsigned int layerIndex = 0; layerIndex < width; layerIndex++){
        for(unsigned int i = 0; i < height; i++){
            layers[layerIndex][i]->save(s);
        }
    }
}

double Network::work(const double *inputs, unsigned int size, NetworkWorkPlace *workPlace)
{
    double result = -1;
    if(inputCount == size){

        for(unsigned int i = 0; i < height;i++){
            workPlace->resultsFromLayers[width-1][i] = layers[width-1][i]->work(inputs,size);
        }
        for(int layerIndex = width-2; layerIndex >= 0; layerIndex--){
            for(unsigned int i = 0; i < height; i++){
                workPlace->resultsFromLayers[layerIndex][i] = layers[layerIndex][i]->work(workPlace->resultsFromLayers[layerIndex+1],height);
            }
        }
        result = lastN->work(workPlace->resultsFromLayers[0],height);
    }
    return result;
}

int Network::getWidth()
{
    return (int)width;
}

int Network::getHeight()
{
    return (int)height;
}

int Network::getInputCount()
{
    return (int)inputCount;
}

