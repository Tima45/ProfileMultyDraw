#include "profilewindow.h"
#include "ui_profilewindow.h"

ProfileWindow::ProfileWindow(   QVector<double> inAngles,
                                QVector<double> inAmpers,
                                double inWireLength,
                                double inLegLength,double inLegsAngle,
                                int inZeroAngleId,
                                bool inShowMirror,
                                double inDeltaX,
                                double inDeltaY,
                                double inPeakX, double inPeakY,
                                double inMiddleLeg1StartX, double inMiddleLeg1StartY,double inMiddleLeg1StopX, double inMiddleLeg1StopY,
                                double inMiddleLeg2StartX,double inMiddleLeg2StartY,double inMiddleLeg2StopX,double inMiddleLeg2StopY,
                                QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ProfileWindow)
{
    ui->setupUi(this);
    angles = inAngles;
    ampers = inAmpers;
    wireLength = inWireLength;
    legLength = inLegLength;
    legsAngle = inLegsAngle;
    showMirror = inShowMirror;
    zeroAngleId = inZeroAngleId;
    deltaX = inDeltaX;
    deltaY = inDeltaY;

    peakX = inPeakX;
    peakY = inPeakY;

    middleLeg1StartX = inMiddleLeg1StartX;
    middleLeg1StartY = inMiddleLeg1StartY;
    middleLeg1StopX = inMiddleLeg1StopX;
    middleLeg1StopY = inMiddleLeg1StopY;

    middleLeg2StartX = inMiddleLeg2StartX;
    middleLeg2StartY = inMiddleLeg2StartY;
    middleLeg2StopX = inMiddleLeg2StopX;
    middleLeg2StopY = inMiddleLeg2StopY;

    initPlot();
    reDraw();
}

ProfileWindow::~ProfileWindow()
{
    delete ui;
}

void ProfileWindow::reDraw()
{
    if(!ampers.isEmpty()){
        int holeDiameter = ui->holeBox->value();
        int pixelsPerMm = ui->pixelsPerMmBox->value();
        //-------------
        double currentMaxAbsValue = 0;
        int maxId = 0;
        for(int i = 0; i < ampers.count(); i++){
            if(fabs(ampers.at(i)) > currentMaxAbsValue){
                currentMaxAbsValue = fabs(ampers.at(i));
                maxId = i;
            }
        }
        double sign = 1;
        if(ampers.at(maxId) < 0){
            sign = -1;
        }
        if(currentMaxAbsValue != 0){
            //------------------------------------------------------
            colorMap->data()->clear();
            colorMap->data()->clear();
            colorMap->data()->setSize(holeDiameter*pixelsPerMm,holeDiameter*pixelsPerMm);

            colorMap->data()->setRange(QCPRange(-holeDiameter/2.0,holeDiameter/2.0),QCPRange(-holeDiameter/2.0,holeDiameter/2.0));
            colorMap->data()->setSize(holeDiameter*pixelsPerMm,holeDiameter*pixelsPerMm);
            colorMap->data()->setRange(QCPRange(-holeDiameter/2.0,holeDiameter/2.0),QCPRange(-holeDiameter/2.0,holeDiameter/2.0));

            for(int i = 0; i < zeroAngleId; i++){
                for(int j = zeroAngleId; j < angles.count(); j++){
                    double x = 0;
                    double y = 0;
                    double a1 = angles.at(i);
                    double a2 = angles.at(j);
                    bool ok = true;
                    Frame::anglesToXY(a1,a2,wireLength,legLength,legsAngle,x,y,ok);
                    if(ok){
                        x += deltaX;
                        y += deltaY;
                        if(showMirror){
                            x *= (-1);
                        }
                        int Xid = 0;
                        int Yid = 0;
                        colorMap->data()->coordToCell(x,y,&Xid,&Yid);
                        double mull = (sign*ampers.at(j) * ampers.at(i))/currentMaxAbsValue;
                        colorMap->data()->setCell(Xid,Yid,mull);
                    }
                }
            }
            colorMap->rescaleDataRange();
            colorScale->setDataRange(QCPRange(-currentMaxAbsValue,currentMaxAbsValue));
            ui->plot->replot();
        }
    }
}

void ProfileWindow::initPlot()
{
    //-----------------------------------------------------------------------
    ui->plot->setInteraction(QCP::iRangeDrag, true);
    ui->plot->setInteraction(QCP::iRangeZoom, true);
    ui->plot->axisRect()->setRangeDrag(Qt::Horizontal | Qt::Vertical);
    ui->plot->axisRect()->setRangeZoom(Qt::Horizontal | Qt::Vertical);
    ui->plot->addLayer("low");
    ui->plot->addLayer("high");
    ui->plot->xAxis->setLabel("Х (мм)");
    ui->plot->yAxis->setLabel("Y (мм)");

    //-----------------------------------------------------------------------
    colorScale = new QCPColorScale(ui->plot);
    colorScale->setType(QCPAxis::atRight);
    g.clearColorStops();
    g.setColorInterpolation(QCPColorGradient::ColorInterpolation::ciRGB);
    g.setColorStopAt(0,QColor(0,0,255));
    g.setColorStopAt(0.5,QColor(255,255,255));
    g.setColorStopAt(1,QColor(255,0,0));
    g.setLevelCount(50);
    colorScale->setGradient(g);
    colorScale->setDataRange(QCPRange(-1,1));
    ui->plot->plotLayout()->addElement(0,1,colorScale);
    //-----------------------------------------------------------------------
    colorMap = new QCPColorMap(ui->plot->xAxis,ui->plot->yAxis);
    colorMap->setColorScale(colorScale);
    colorMap->setTightBoundary(true);
    ui->plot->addPlottable(colorMap);
    colorMap->setLayer("low");
    ui->plot->xAxis->setRange(-30,30);
    ui->plot->yAxis->setRange(-30,30);
    //-----------------------------------------------------------------------
    profilometrAxis = new QCPItemLine(ui->plot);
    QPen axP;
    axP.setColor(QColor(Qt::black));
    axP.setStyle(Qt::PenStyle::DashLine);
    profilometrAxis->setPen(axP);
    profilometrAxis->start->setCoords((showMirror)?(-deltaX):(deltaX),deltaY);
    profilometrAxis->end->setCoords((showMirror)?(-deltaX):(deltaX),deltaY+wireLength);
    ui->plot->addItem(profilometrAxis);
    profilometrAxis->setLayer("high");
    //-----------------------------------------------------------------------
    middleLeg1 = new QCPItemLine(ui->plot);
    middleLeg2 = new QCPItemLine(ui->plot);
    middleLeg1->setPen(axP);
    middleLeg2->setPen(axP);
    ui->plot->addItem(middleLeg1);
    ui->plot->addItem(middleLeg2);
    middleLeg1->setLayer("high");
    middleLeg2->setLayer("high");

    middleLeg1->start->setCoords((showMirror)?(-middleLeg1StartX):(middleLeg1StartX),middleLeg1StartY);
    middleLeg1->end->setCoords((showMirror)?(-middleLeg1StopX):(middleLeg1StopX),middleLeg1StopY);
    middleLeg2->start->setCoords((showMirror)?(-middleLeg2StartX):(middleLeg2StartX),middleLeg2StartY);
    middleLeg2->end->setCoords((showMirror)?(-middleLeg2StopX):(middleLeg2StopX),middleLeg2StopY);
    //-----------------------------------------------------------------------
    hole = new QCPItemEllipse(ui->plot);
    QPen p1;
    p1.setWidth(2);
    p1.setColor(QColor(Qt::green));
    p1.setStyle(Qt::PenStyle::DashLine);
    hole->setPen(p1);

    hole->topLeft->setCoords(-ui->holeBox->value()/2.0,ui->holeBox->value()/2.0);
    hole->bottomRight->setCoords(ui->holeBox->value()/2.0,-ui->holeBox->value()/2.0);
    ui->plot->addItem(hole);
    hole->setLayer("high");
    //-----------------------------------------------------------------------
    center = new QCPItemTracer(ui->plot);
    center->position->setCoords(0,0);
    center->setStyle(QCPItemTracer::tsPlus);
    ui->plot->addItem(center);
    center->setLayer("high");
    //-----------------------------------------------------------------------
    peak = new QCPItemTracer(ui->plot);
    peak->position->setCoords(0,0);
    peak->setStyle(QCPItemTracer::tsCircle);
    peak->setPen(QPen(QColor(255,255,0,200)));
    peak->setBrush(QBrush(QColor(255,255,0,100)));
    peak->position->setCoords(peakX,peakY);
    ui->plot->addItem(peak);
    peak->setLayer("high");
}

void ProfileWindow::on_holeBox_valueChanged(int arg1)
{
    Q_UNUSED(arg1);
    hole->topLeft->setCoords(-ui->holeBox->value()/2.0,ui->holeBox->value()/2.0);
    hole->bottomRight->setCoords(ui->holeBox->value()/2.0,-ui->holeBox->value()/2.0);
    reDraw();
}

void ProfileWindow::on_pixelsPerMmBox_valueChanged(int arg1)
{
    Q_UNUSED(arg1);
    reDraw();
}

void ProfileWindow::on_spinBox_valueChanged(int arg1)
{
    g.setLevelCount(arg1);
    colorScale->setGradient(g);
    ui->plot->replot();
}

void ProfileWindow::on_pushButton_clicked()
{
    QString s = QFileDialog::getSaveFileName(this,"Сохранить скриншот","","png");
    ui->plot->grab().save(s+".png","png");
}
