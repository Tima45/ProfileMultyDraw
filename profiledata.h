#ifndef PROFILEDATA_H
#define PROFILEDATA_H

#include <QObject>
#include <QtCore>
#include <QMessageBox>
#include <QRegExp>
#include <waveletspectrum.h>
#include "plot/qcustomplot.h"
#include "frame.h"
#include "profileinfoblock.h"
#include "profilewindow.h"

class ProfileData : public QObject
{
    Q_OBJECT
public:
    ProfileData(QObject *parent = 0);
    ProfileData(QString inFileName, QCustomPlot *inPlot, QScrollArea *inScrollArea, QColor inColor, double wireLength, double legLength,double legWidth,double legsAngle, double deltaX, double deltaY, double luftAngle,bool showMirror,double inAngleToMMKoef, QObject *parent);
    ~ProfileData();
    QString fileName;
    QColor color;
signals:
    void wantToBeDeleted(QString fileName);
public slots:
    void deletePressed();
    void showFrame(int id);
    void changePart(double value);
    void showMaxCheck(bool);
    void showIntervalCheck(bool);
    void showMiddleCheck(bool);
    void redrawAll(QColor);
    void showProfileWindow();
    void unseeit();
    void changeLineSize(int value);
    void changeLineType(Qt::PenStyle);
    void denoiseCurrentFrame(int percent);
private:
    QCustomPlot *plot;
    QCPGraph *ampersGraph;
    QCPGraph *realAmpersGraph;
    QPen mainPen;

    QCPItemLine *maxCurrentLeg1Line;
    QCPItemLine *maxCurrentLeg2Line;
    QCPItemLine *middleLineLeg1;
    QCPItemLine *middleLineLeg2;
    QCPGraph *massZoneLeg1;
    QCPGraph *massZoneLeg2;
    QCPItemLine *massMiddleLeg1;
    QCPItemLine *massMiddleLeg2;
    QCPItemTracer *massTracerLeg1;
    QCPItemTracer *massTracerLeg2;
    void addToPlot();
    void deleteFromPlot();
    //------------------------------------
    QScrollArea *scrollArea;
    ProfileInfoBlock *infoBlock;
    bool profileWindowIsShown = false;
    void addToScrollArea();
    void deleteFromScrollArea();
    //------------------------------------
    QVector<int> borders;
    QVector<double> rawAmpers;
    QVector<double> rawAngles;
    QVector<Frame *> frames;
    int currentFrameId = -1;
    QString scanDate;

    void readData(QString fileName);
    void findBorders();
    void loadFrames(double wireLength, double legLength, double legWidth, double deltaX, double deltaY, double luftAngle, double legsAngle, bool showMirror);
    QVector<double> denoiseAngles(QVector<double> angles);

    double angleToMMKoef;

};

#endif // PROFILEDATA_H
