#include "profilewindow.h"
#include "ui_profilewindow.h"

ProfileWindow::ProfileWindow(QVector<double> inAngles,
                                QVector<double> inAmpers,
                                double inWireLength,
                                double inLegLength, double inLegsAngle,
                                int inZeroAngleId,
                                bool inShowMirror,
                                double inDeltaX,
                                double inDeltaY,
                                double inPeakX, double inPeakY,
                                double inMiddleLeg1StartX, double inMiddleLeg1StartY, double inMiddleLeg1StopX, double inMiddleLeg1StopY,
                                double inMiddleLeg2StartX, double inMiddleLeg2StartY, double inMiddleLeg2StopX, double inMiddleLeg2StopY, void *profileGenerator,
                                QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ProfileWindow)
{
    generator = (ProfileGenerator*)profileGenerator;
    ui->setupUi(this);
    profileIsCorrect = false;
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
    buildNetworkColorMap();
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
            if(ui->mulRadio->isChecked()){
                colorMapNetwork->setVisible(false);
                colorMap->setVisible(true);
                //------------------------------------------------------
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

            }
            if(ui->networkRadio->isChecked()){
                if(profileIsCorrect){
                    colorMapNetwork->setVisible(true);
                    colorMap->setVisible(false);
                    colorMapNetwork->data()->clear();
                    colorMapNetwork->data()->setSize(holeDiameter*pixelsPerMm,holeDiameter*pixelsPerMm);

                    colorMapNetwork->data()->setRange(QCPRange(-holeDiameter/2.0,holeDiameter/2.0),QCPRange(-holeDiameter/2.0,holeDiameter/2.0));
                    colorMapNetwork->data()->setSize(holeDiameter*pixelsPerMm,holeDiameter*pixelsPerMm);
                    colorMapNetwork->data()->setRange(QCPRange(-holeDiameter/2.0,holeDiameter/2.0),QCPRange(-holeDiameter/2.0,holeDiameter/2.0));
                    double yR = 64.0/(bordersIndex.at(1) - bordersIndex.at(0));
                    double xR = 64.0/(bordersIndex.at(3) - bordersIndex.at(2));

                    for(int i = bordersIndex.at(0); i < bordersIndex.at(1); i++){
                        for(int j = bordersIndex.at(2); j < bordersIndex.at(3); j++){

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

                                int picY = qRound((i-bordersIndex.at(0))*yR);
                                int picX = qRound((j-bordersIndex.at(2))*xR);

                                colorMapNetwork->data()->coordToCell(x,y,&Xid,&Yid);
                                colorMapNetwork->data()->setCell(Xid,Yid,sign*pic[picY][picX]*currentMaxAbsValue);
                            }
                        }
                    }
                    colorMapNetwork->rescaleDataRange();
                }
            }
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
    ui->plot->moveLayer(ui->plot->layer("grid"),ui->plot->layer("high"));
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

    colorMapNetwork = new QCPColorMap(ui->plot->xAxis,ui->plot->yAxis);
    colorMapNetwork->setColorScale(colorScale);
    colorMapNetwork->setTightBoundary(true);
    ui->plot->addPlottable(colorMapNetwork);
    colorMapNetwork->setLayer("low");
    colorMapNetwork->setVisible(false);
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

void ProfileWindow::buildNetworkColorMap()
{
    double currentMaxAbsValue = 0;
    for(int i = 0; i < ampers.count(); i++){
        if(fabs(ampers.at(i)) > currentMaxAbsValue){
            currentMaxAbsValue = fabs(ampers.at(i));
        }
    }

    for(int i = 1; i < ampers.count(); i++){
        int shakalValue1 = (int)(ampers.at(i-1)/currentMaxAbsValue*5);
        int shakalValue2 = (int)(ampers.at(i)/currentMaxAbsValue*5);
        if((shakalValue1 == 0 && shakalValue2 != 0)){
            bordersIndex.append(i-1);
        }
        if((shakalValue2 == 0 && shakalValue1 != 0)){
            bordersIndex.append(i);
        }
    }
    if(bordersIndex.count() == 4){
        if((bordersIndex.at(0) < bordersIndex.at(1)) &&
           (bordersIndex.at(2) < bordersIndex.at(3)) &&
           (bordersIndex.at(1) < bordersIndex.at(2)) &&
           (bordersIndex.at(1) < zeroAngleId) &&
           (bordersIndex.at(2) > zeroAngleId))
        {
            QVector<double> xProfile;
            QVector<double> yProfile;
            QVector<double> xyProfile;


            for(int i = bordersIndex.at(0); i < bordersIndex.at(1); i++){
                yProfile.append(fabs(ampers.at(i)));
            }
            for(int i = bordersIndex.at(2); i < bordersIndex.at(3); i++){
                xProfile.append(fabs(ampers.at(i)));
            }

            if(yProfile.count() > 2 && xProfile.count() > 2){
                profileIsCorrect = true;
                ui->networkRadio->setChecked(true);

                double minAbs = fabs(xProfile.first());
                for(int i = 1; i < xProfile.count(); i++){
                    if(fabs(xProfile.at(i)) < minAbs){
                        minAbs = fabs(xProfile.at(i));
                    }
                }
                for(int i = 0; i < xProfile.count(); i++){
                    xProfile[i] -= minAbs;
                }

                minAbs = fabs(yProfile.first());
                for(int i = 1; i < yProfile.count(); i++){
                    if(fabs(yProfile.at(i)) < minAbs){
                        minAbs = fabs(yProfile.at(i));
                    }
                }
                for(int i = 0; i < yProfile.count(); i++){
                    yProfile[i] -= minAbs;
                }

                resize(xProfile,64);
                resize(yProfile,64);

                /*
                xProfile.prepend(0);
                xProfile.prepend(0);
                xProfile.append(0);
                xProfile.append(0);
                yProfile.prepend(0);
                yProfile.prepend(0);
                yProfile.append(0);
                yProfile.append(0);*/

                xyProfile.append(xProfile);
                xyProfile.append(yProfile);


                double max = 0;
                for(int i = 0; i < xyProfile.count(); i++){
                    if(fabs(xyProfile.at(i)) > max){
                        max = fabs(xyProfile.at(i));
                    }
                }
                for(int i = 0; i < xyProfile.count(); i++){
                    xyProfile[i] /= max;
                }

                for(int i = 0; i < 64; i++){
                    pic.append(QVector<double>());
                    for(int j = 0; j < 64; j++){
                        pic.last().append(0);
                    }
                }
                generator->generate(pic,xyProfile);


                QVector<double> xyProfileResult;

                for(int x = 0; x < 64; x++){
                    double summ = 0;
                    for(int y = 0; y < 64; y++){
                        summ += pic[y][x];
                    }
                    xyProfileResult.append(summ);
                }
                for(int y = 0; y < 64; y++){
                    double summ = 0;
                    for(int x = 0; x < 64; x++){
                        summ += pic[y][x];
                    }
                    xyProfileResult.append(summ);
                }
                double maxResult = 0;
                for(int i = 0; i < xyProfileResult.count(); i++){
                    if(xyProfileResult.at(i) > maxResult){
                        maxResult = xyProfileResult.at(i);
                    }
                }
                for(int i = 0; i < xyProfileResult.count(); i++){
                    xyProfileResult[i] /= maxResult;
                }

                double error = 0;
                for(int i = 0; i < xyProfile.count(); i++){
                    error += fabs(xyProfile.at(i)-xyProfileResult.at(i));
                }
                error /= 256;
                error *= 100;

                ui->errorLabel->setText(QString::number(fabs(100.0-error)));
            }else{
                ui->networkRadio->setCheckable(false);
                ui->errorLabel->setText("--");
            }
        }else{
            ui->networkRadio->setCheckable(false);
            ui->errorLabel->setText("--");
        }
    }else{
        ui->networkRadio->setCheckable(false);
        ui->errorLabel->setText("--");
    }
}

void ProfileWindow::resize(QVector<double> &vector, int size)
{
    /*
    if(vector.count() > size){
        qDebug() << "lol!";
    }*/

    if(!vector.isEmpty()){
        int lastSize = vector.count();
        double *x = new double[lastSize];
        for(int i = 0; i < lastSize; i++){
            x[i] = i;
        }
        gsl_interp_accel *acc = gsl_interp_accel_alloc();
        gsl_spline *spline = gsl_spline_alloc(gsl_interp_cspline,lastSize);
        gsl_spline_init(spline,x,vector.constData(),lastSize);

        vector.clear();
        for(int i = 0; i < size; i++){
            double yi = gsl_spline_eval(spline,i*(lastSize/(double)size), acc);
            yi < 0? yi = 0 : yi;
            vector.append(yi);
        }

        gsl_spline_free(spline);
        gsl_interp_accel_free(acc);
    }
}

void ProfileWindow::on_mulRadio_clicked()
{
    reDraw();
}

void ProfileWindow::on_networkRadio_clicked()
{
    reDraw();
}
