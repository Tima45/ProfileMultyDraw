#include "networkworkplace.h"

NetworkWorkPlace::NetworkWorkPlace(unsigned int inputCount, unsigned int width, unsigned int height) :
    width(width)
{
    resultsFromLayers = new double*[width];
    for(unsigned int layerIndex = 0; layerIndex < width; layerIndex++){
        resultsFromLayers[layerIndex] = new double[height];
    }

    errorsFromLayers = new double*[width-1];
    for(unsigned int layerIndex = 0; layerIndex < width-1; layerIndex++){
        errorsFromLayers[layerIndex] = new double[height];
    }
}

NetworkWorkPlace::~NetworkWorkPlace()
{
    for(unsigned int layerIndex = 0; layerIndex < width; layerIndex++){
        delete[] resultsFromLayers[layerIndex];
    }
    delete[] resultsFromLayers;

    for(unsigned int layerIndex = 0; layerIndex < width-1; layerIndex++){
        delete[] errorsFromLayers[layerIndex];
    }
    delete[] errorsFromLayers;
}
