#ifndef FRAME_H
#define FRAME_H

#include <QObject>
#include <QtCore>
#include "waveletspectrum.h"

class Frame : public QObject
{
    Q_OBJECT    
public:
    enum Direction{
        goDown,
        goUp,
    };
    static void anglesToXY(double inA1,double inA2,double wireLength,double legLength,double legsAngle,double &outX,double &outY,bool &ok);
    explicit Frame(QVector<double> *inRawAngles, QVector<double> *inRawAmpers, Direction inDir, int startKey, int stopKey, double inWireLength, double inLegLength, double inLegWidth, double inLegsAngle, double inDeltaX, double inDeltaY, double inLuftAngle, bool inshowMirror, QObject *parent = 0);
    QVector<double> angles;
    QVector<double> ampers;

    QVector<double> smoothAmpers;
    QVector<double> smoothAnglesDiffs;
    int noiseReductionPercent = 0;

    void loadData(QVector<double> *inRawAngles,QVector<double> *inRawAmpers);
    int startIndex;
    int stopIndex;
    int zeroAngleIndex;
    void findZeroAngleIndex();
    //------------------------------------
    Direction dir;
    double wireLength;
    double legLength;
    double legWidth;
    double deltaX;
    double deltaY;
    double luftAngle;
    double legsAngle;
    bool showMirror;
    //------------------------------------
    //Максимум
    void findMaxCurrentPosition();
    bool ok;
    double maxCurrentAngle1; double maxAmperLeg1;
    double maxCurrentAngle2; double maxAmperLeg2;
    int maxCurrentIndex1;
    int maxCurrentIndex2;
    double maxCurrentX;
    double maxCurrentY;
    //------------------------------------
    //Ширина на полувысоте
    void findWidth();
    double widthLeg1;
    double widthLeg2;

    double Leg1AngleStart = 0;
    double Leg1AngleStop = 0;
    double Leg2AngleStart = 0;
    double Leg2AngleStop = 0;

    double LineLeg1StartX;
    double LineLeg1StartY;
    double LineLeg1StopX;
    double LineLeg1StopY;
    double LineLeg2StartX;
    double LineLeg2StartY;
    double LineLeg2StopX;
    double LineLeg2StopY;
    //------------------------------------
    //Общий ток
    void findCurrent();
    double currentLeg1;
    double currentLeg2;
    double current;
    //------------------------------------
    //Положение центра масс и ток
    void findMass(double partValue);
    void findPart(double percentOfBeam);
    double part = 2.7182;
    double massX;
    double massY;

    double massWidthLeg1;
    double massWidthLeg2;

    double massLineLeg1StartX;
    double massLineLeg1StartY;
    double massLineLeg1StopX;
    double massLineLeg1StopY;
    double massLineLeg2StartX;
    double massLineLeg2StartY;
    double massLineLeg2StopX;
    double massLineLeg2StopY;


    double massLeg1AngleStart = 0;
    double massLeg1AngleStop = 0;
    double massMiddleAmperLeg1 = 0;

    double massLeg2AngleStart = 0;
    double massLeg2AngleStop = 0;
    double massMiddleAmperLeg2 = 0;

    double currentLeg1Mass;
    double currentLeg2Mass;
    double currentMass;

    double difCurrentLeg1;
    double difCurrentLeg2;
    double difCurrent = 0;

private:
    double countCurrent(int start,int stop);
signals:

public slots:
    void changePart(double value);
    void setNoiseReductionPercent(int percent);
    void denoiseAmpers();
};

#endif // FRAME_H

