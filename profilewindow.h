#ifndef PROFILEWINDOW_H
#define PROFILEWINDOW_H

#include <QDialog>
#include <QDebug>
#include "plot/qcustomplot.h"
#include "frame.h"
#include "profilegenerator.h"
#include <gsl/gsl_spline.h>

namespace Ui {
class ProfileWindow;
}

class ProfileWindow : public QDialog
{
    Q_OBJECT

public:
    explicit ProfileWindow(QVector<double> inAngles,
                           QVector<double> inAmpers,
                           double inWireLength,
                           double inLegLength, double inLegsAngle,
                           int inZeroAngleId,
                           bool inShowMirror,
                           double inDeltaX,
                           double inDeltaY,
                           double inPeakX, double inPeakY,
                           double inMiddleLeg1StartX, double inMiddleLeg1StartY, double inMiddleLeg1StopX, double inMiddleLeg1StopY,
                           double inMiddleLeg2StartX, double inMiddleLeg2StartY, double inMiddleLeg2StopX, double inMiddleLeg2StopY,
                           void *profileGenerator,
                           QWidget *parent = 0);
    ~ProfileWindow();

    double wireLength;
    double legLength;
    bool showMirror;
    int zeroAngleId;
    double deltaX;
    double deltaY;
    double legsAngle;

    double peakX,peakY;
    double middleLeg1StartX,middleLeg1StartY,middleLeg1StopX,middleLeg1StopY;
    double middleLeg2StartX,middleLeg2StartY,middleLeg2StopX,middleLeg2StopY;

    QVector<double> angles;
    QVector<double> ampers;
private slots:
    void on_holeBox_valueChanged(int arg1);

    void on_pixelsPerMmBox_valueChanged(int arg1);

    void on_spinBox_valueChanged(int arg1);

    void on_pushButton_clicked();

    void on_mulRadio_clicked();

    void on_networkRadio_clicked();

private:
    Ui::ProfileWindow *ui;

    QCPColorGradient g;
    QCPColorScale *colorScale;
    QCPColorMap *colorMap;

    QCPColorMap *colorMapNetwork;
    bool profileIsCorrect = false;

    QCPItemEllipse *hole;
    QCPItemTracer *center;
    QCPItemTracer *peak;
    QCPItemLine *profilometrAxis;
    QCPItemLine *middleLeg1;
    QCPItemLine *middleLeg2;


    QVector<int> bordersIndex;
    ProfileGenerator *generator;
    QVector<QVector<double>> pic;



    void buildNetworkColorMap();
    void reDraw();
    void initPlot();
    void resize(QVector<double> &vector, int size);
};

#endif // PROFILEWINDOW_H
