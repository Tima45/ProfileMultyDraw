#ifndef NETWORKWORKPLACE_H
#define NETWORKWORKPLACE_H


class NetworkWorkPlace
{
public:
    NetworkWorkPlace(unsigned int inputCount,unsigned int width, unsigned int height);
    ~NetworkWorkPlace();
    double **resultsFromLayers;
    double **errorsFromLayers;
private:
    unsigned int width = 0;
    NetworkWorkPlace(const &NetworkWorkPlace);
};

#endif // NETWORKWORKPLACE_H
