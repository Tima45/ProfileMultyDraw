#include "frame.h"


void Frame::anglesToXY(double inA1,double inA2,double wireLength,double legLength,double legsAngle,double &outX,double &outY,bool &ok){
    ok = true;

    double a1 = inA1*M_PI/180.0;
    double a2 = inA2*M_PI/180.0;

    double as1 = M_PI_2-a1;
    double as2 = M_PI_2-a2;

    double Xc1 = wireLength*cos(as1);
    double Yc1 = wireLength*sin(as1);

    double Xc2 = wireLength*cos(as2);
    double Yc2 = wireLength*sin(as2);

    double ac1 = M_PI - ((1-legsAngle/360.0)*M_PI - as1);
    double ac2 = (1-legsAngle/360.0)*M_PI - (M_PI - as2);

    double cosC1 = cos(ac1);
    double sinC1 = sin(ac1);

    double cosC2 = cos(ac2);
    double sinC2 = sin(ac2);

    double Lc1 = (Yc1-Yc2-((Xc1*sinC2)/(cosC2))+((Xc2*sinC2)/(cosC2)))/(((cosC1*sinC2)/(cosC2))-sinC1);
    double Lc2 = (Xc1-Xc2+Lc1*cosC1)/(cosC2);
    if(Lc1 <= legLength && Lc2 <= legLength){
        outX = Xc1 + Lc1*cosC1;
        outY = Yc1 + Lc1*sinC1;
    }else{
        ok = false;
        outX = 0;
        outY = 0;
    }
}


Frame::Frame(QVector<double> *inRawAngles, QVector<double> *inRawAmpers, Direction inDir, int startKey, int stopKey, double inWireLength, double inLegLength, double inLegWidth,double inLegsAngle, double inDeltaX, double inDeltaY, double inLuftAngle, bool inshowMirror, QObject *parent) : QObject(parent)
{
    dir = inDir;
    startIndex = startKey;
    stopIndex = stopKey;
    luftAngle = inLuftAngle;
    showMirror = inshowMirror;
    legsAngle = inLegsAngle;

    loadData(inRawAngles,inRawAmpers);
    findZeroAngleIndex();

    wireLength = inWireLength;
    legLength = inLegLength;
    legWidth = inLegWidth;
    deltaX = inDeltaX;
    deltaY = inDeltaY;

    denoiseAmpers();
    findMaxCurrentPosition();
    findWidth();
    findCurrent();
    findPart(95);
}

void Frame::loadData(QVector<double> *inRawAngles,QVector<double> *inRawAmpers)
{
    ampers.clear();
    angles.clear();
    if(!inRawAngles->isEmpty() && !inRawAmpers->isEmpty()){
        if(dir == goDown){
            for(int i = startIndex; i < stopIndex; i++){
                ampers.append(inRawAmpers->at(i));
                angles.append(inRawAngles->at(i)-(luftAngle/2.0));
            }
        }else if(dir == goUp){
            for(int i = stopIndex; i >= startIndex; i--){
                ampers.append(inRawAmpers->at(i));
                angles.append(inRawAngles->at(i)+(luftAngle/2.0));
            }
        }
    }
}

void Frame::findZeroAngleIndex()
{
    if(!angles.isEmpty()){
        double minAbs = fabs(angles.at(0));
        zeroAngleIndex = 0;
        for(int i = 1; i < angles.count(); i++){
            if(fabs(angles.at(i)) < minAbs){
                minAbs = fabs(angles.at(i));
                zeroAngleIndex = i;
            }
        }
    }
}

void Frame::findMaxCurrentPosition()
{
    if(!angles.isEmpty() && !smoothAmpers.isEmpty()){
        double maxAbsValue1 = 0;
        maxCurrentAngle1 = -1;
        for(int i = 0; i < zeroAngleIndex; i++){
            if(fabs(smoothAmpers.at(i)) > maxAbsValue1){
                maxAbsValue1 = fabs(smoothAmpers.at(i));
                maxCurrentAngle1 = angles.at(i);
                maxCurrentIndex1 = i;
            }
        }
        double maxAbsValue2 = 0;
        for(int i = zeroAngleIndex; i < smoothAmpers.count(); i++){
            if(fabs(smoothAmpers.at(i)) > maxAbsValue2){
                maxAbsValue2 = fabs(smoothAmpers.at(i));
                maxCurrentAngle2 = angles.at(i);
                maxCurrentIndex2 = i;
            }
        }
        maxAmperLeg1 = smoothAmpers.at(maxCurrentIndex1);
        maxAmperLeg2 = smoothAmpers.at(maxCurrentIndex2);
        anglesToXY(maxCurrentAngle1,maxCurrentAngle2,wireLength,legLength,legsAngle,maxCurrentX,maxCurrentY,ok);
        maxCurrentX += deltaX;
        maxCurrentY += deltaY;

        if(showMirror){
            maxCurrentX = -1.0*maxCurrentX;
        }

    }
}

void Frame::findWidth()
{
    if(ok && !smoothAmpers.isEmpty() && !angles.isEmpty()){
        double halfLeg1 = smoothAmpers.at(maxCurrentIndex1)/2.0;
        double halfLeg2 = smoothAmpers.at(maxCurrentIndex2)/2.0;

        QVector<double> anglesOfMiddleLeg1;
        QVector<double> anglesOfMiddleLeg2;
        for(int i = 1; i < zeroAngleIndex; i++){
            if((smoothAmpers.at(i-1) >= halfLeg1 && smoothAmpers.at(i) <= halfLeg1) || (smoothAmpers.at(i) >= halfLeg1 && smoothAmpers.at(i-1) <= halfLeg1)){
                anglesOfMiddleLeg1.append(angles.at(i));
            }
        }
        for(int i = zeroAngleIndex; i < smoothAmpers.count(); i++){
            if((smoothAmpers.at(i-1) >= halfLeg2 && smoothAmpers.at(i) <= halfLeg2) || (smoothAmpers.at(i) >= halfLeg2 && smoothAmpers.at(i-1) <= halfLeg2)){
                anglesOfMiddleLeg2.append(angles.at(i));
            }
        }
        //-----------------------------------------------------
        if(anglesOfMiddleLeg1.count() >= 2 && anglesOfMiddleLeg2.count() >= 2){

            Leg1AngleStart = anglesOfMiddleLeg1.first();
            Leg1AngleStop = anglesOfMiddleLeg1.last();
            Leg2AngleStart = anglesOfMiddleLeg2.first();
            Leg2AngleStop = anglesOfMiddleLeg2.last();

            bool ook;
            anglesToXY(anglesOfMiddleLeg1.first(),maxCurrentAngle2,wireLength,legLength,legsAngle,LineLeg1StartX,LineLeg1StartY,ook);
            LineLeg1StartX += deltaX;
            LineLeg1StartY += deltaY;

            anglesToXY(anglesOfMiddleLeg1.last(),maxCurrentAngle2,wireLength,legLength,legsAngle,LineLeg1StopX,LineLeg1StopY,ook);
            LineLeg1StopX += deltaX;
            LineLeg1StopY += deltaY;

            anglesToXY(maxCurrentAngle1,anglesOfMiddleLeg2.first(),wireLength,legLength,legsAngle,LineLeg2StartX,LineLeg2StartY,ook);
            LineLeg2StartX += deltaX;
            LineLeg2StartY += deltaY;

            anglesToXY(maxCurrentAngle1,anglesOfMiddleLeg2.last(),wireLength,legLength,legsAngle,LineLeg2StopX,LineLeg2StopY,ook);
            LineLeg2StopX += deltaX;
            LineLeg2StopY += deltaY;

            widthLeg1 = sqrt(powf((LineLeg1StartX-LineLeg1StopX),2)+powf((LineLeg1StartY-LineLeg1StopY),2));
            widthLeg2 = sqrt(powf((LineLeg2StartX-LineLeg2StopX),2)+powf((LineLeg2StartY-LineLeg2StopY),2));
        }
    }
}

void Frame::findCurrent()
{
    if(!ampers.isEmpty() && !angles.isEmpty()){
        currentLeg1 = countCurrent(0,zeroAngleIndex);
        currentLeg2 = countCurrent(zeroAngleIndex,ampers.count());
        current = (currentLeg1+currentLeg2)/2.0;
    }
}

void Frame::findMass(double partValue)
{
    if(!angles.isEmpty() && !smoothAmpers.isEmpty()){
        if(partValue > 1){
            part = partValue;
            double halfLeg1 = smoothAmpers.at(maxCurrentIndex1)/partValue;
            double halfLeg2 = smoothAmpers.at(maxCurrentIndex2)/partValue;
            massMiddleAmperLeg1 = halfLeg1;
            massMiddleAmperLeg2 = halfLeg2;


            QVector<int> anglesIndexOfMiddleLeg1;
            QVector<int> anglesIndexOfMiddleLeg2;
            for(int i = 1; i < zeroAngleIndex; i++){
                if((smoothAmpers.at(i-1) >= halfLeg1 && smoothAmpers.at(i) <= halfLeg1) || (smoothAmpers.at(i) >= halfLeg1 && smoothAmpers.at(i-1) <= halfLeg1)){
                    anglesIndexOfMiddleLeg1.append(i);
                }
            }
            for(int i = zeroAngleIndex; i < smoothAmpers.count(); i++){
                if((smoothAmpers.at(i-1) >= halfLeg2 && smoothAmpers.at(i) <= halfLeg2) || (smoothAmpers.at(i) >= halfLeg2 && smoothAmpers.at(i-1) <= halfLeg2)){
                    anglesIndexOfMiddleLeg2.append(i);
                }
            }
            if(anglesIndexOfMiddleLeg1.count() >= 2 && anglesIndexOfMiddleLeg2.count() >= 2){
                massLeg1AngleStart = angles.at(anglesIndexOfMiddleLeg1.first());
                massLeg1AngleStop = angles.at(anglesIndexOfMiddleLeg1.last());
                massLeg2AngleStart = angles.at(anglesIndexOfMiddleLeg2.first());
                massLeg2AngleStop = angles.at(anglesIndexOfMiddleLeg2.last());              

                double angle1 = (massLeg1AngleStart+massLeg1AngleStop)/2.0;
                double angle2 = (massLeg2AngleStart+massLeg2AngleStop)/2.0;
                bool ook;
                anglesToXY(angle1,angle2,wireLength,legLength,legsAngle,massX,massY,ook);
                massX += deltaX;
                massY += deltaY;
                //----------------------------
                anglesToXY(massLeg1AngleStart,maxCurrentAngle2,wireLength,legLength,legsAngle,massLineLeg1StartX,massLineLeg1StartY,ook);
                massLineLeg1StartX += deltaX;
                massLineLeg1StartY += deltaY;

                anglesToXY(massLeg1AngleStop,maxCurrentAngle2,wireLength,legLength,legsAngle,massLineLeg1StopX,massLineLeg1StopY,ook);
                massLineLeg1StopX += deltaX;
                massLineLeg1StopY += deltaY;

                anglesToXY(maxCurrentAngle1,massLeg2AngleStart,wireLength,legLength,legsAngle,massLineLeg2StartX,massLineLeg2StartY,ook);
                massLineLeg2StartX += deltaX;
                massLineLeg2StartY += deltaY;

                anglesToXY(maxCurrentAngle1,massLeg2AngleStop,wireLength,legLength,legsAngle,massLineLeg2StopX,massLineLeg2StopY,ook);
                massLineLeg2StopX += deltaX;
                massLineLeg2StopY += deltaY;

                massWidthLeg1 = sqrt(powf((massLineLeg1StartX-massLineLeg1StopX),2)+powf((massLineLeg1StartY-massLineLeg1StopY),2));
                massWidthLeg2 = sqrt(powf((massLineLeg2StartX-massLineLeg2StopX),2)+powf((massLineLeg2StartY-massLineLeg2StopY),2));
                //----------------------------
                if(showMirror){
                    massX = -1.0*massX;
                }
                currentLeg1Mass = countCurrent(anglesIndexOfMiddleLeg1.first(),anglesIndexOfMiddleLeg1.last());
                currentLeg2Mass = countCurrent(anglesIndexOfMiddleLeg2.first(),anglesIndexOfMiddleLeg2.last());
                currentMass = (currentLeg1Mass+currentLeg2Mass)/2.0;

                difCurrentLeg1 = currentLeg1Mass*100.0/currentLeg1;
                difCurrentLeg2 = currentLeg2Mass*100.0/currentLeg2;
                difCurrent = currentMass*100.0/current;
            }
        }
    }
}

void Frame::findPart(double percentOfBeam)
{
    part = 1.0;
    while(difCurrent < percentOfBeam && part < 100){
        part += 0.1;
        findMass(part);
    }
    if(part >= 100){
        part = 2;
    }
}

double Frame::countCurrent(int start, int stop)
{
    //===================================================
    double result = 0;
    if(!ampers.isEmpty()){
        double r1 = sqrt(wireLength*wireLength + legLength*legLength - 2*wireLength*legLength*cos((1-legsAngle/360.0)*M_PI));
        result += 1000*ampers.at(start);
        for(int i = start+1;i < stop; i++){
            double deltaFi = fabs(angles.at(i-1) - angles.at(i));
            double squWay = (M_PI*r1*r1*deltaFi/360.0)-(M_PI*wireLength*wireLength*deltaFi/360.0);
            double squLeg = legLength*legWidth;
            result += 1000*ampers.at(i)*squWay/squLeg;
        }
    }
    return result;
}

void Frame::changePart(double value)
{
    findMass(value);
}

void Frame::setNoiseReductionPercent(int percent)
{
    noiseReductionPercent = percent;
    denoiseAmpers();

    findMaxCurrentPosition();
    findWidth();
    findCurrent();
    findMass(part);
}

void Frame::denoiseAmpers()
{
    if(!ampers.isEmpty()){
        smoothAmpers.clear();
        WaveletSpectrum *DWT = new WaveletSpectrum(ampers,WaveletSpectrum::BSPLINE_309);
        DWT->threshold(noiseReductionPercent/100.0);
        smoothAmpers = DWT->toData();
        delete DWT;
    }
}
