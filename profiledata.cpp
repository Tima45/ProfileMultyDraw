#include "profiledata.h"


ProfileData::ProfileData(QObject *parent) : QObject(parent)
{

}

ProfileData::ProfileData(QString inFileName, QCustomPlot *inPlot, QScrollArea *inScrollArea, QColor inColor, double wireLength, double legLength, double legWidth, double legsAngle, double deltaX, double deltaY, double luftAngle, bool showMirror, double inAngleToMMKoef, QObject *parent) : QObject(parent)
{
    fileName = inFileName;
    plot = inPlot;
    scrollArea = inScrollArea;
    color = inColor;
    angleToMMKoef = inAngleToMMKoef;

    readData(fileName);
    findBorders();
    loadFrames(wireLength,legLength,legWidth,deltaX,deltaY,luftAngle,legsAngle,showMirror);

    addToPlot();
    addToScrollArea();
    showFrame(0);
}

ProfileData::~ProfileData()
{
    deleteFromPlot();
    deleteFromScrollArea();
}

void ProfileData::deletePressed()
{
    emit wantToBeDeleted(this->fileName);
}

void ProfileData::showFrame(int id)
{
    if(!frames.isEmpty() && id >= 0 && id < frames.count()){
        /*
        QCPRange xR = plot->xAxis->range();
        QCPRange yR = plot->yAxis->range();
*/
        currentFrameId = id;
        Frame* currentFrame = frames.at(currentFrameId);
        //--------------
        ampersGraph->clearData();
        ampersGraph->setData(currentFrame->angles,currentFrame->smoothAmpers);

        realAmpersGraph->clearData();
        realAmpersGraph->setData(currentFrame->angles,currentFrame->ampers);
        //----------------------------
        QVector<double> x1,y1,x2,y2;
        x1.append(currentFrame->massLeg1AngleStart);
        x1.append(currentFrame->massLeg1AngleStop);
        x2.append(currentFrame->massLeg2AngleStart);
        x2.append(currentFrame->massLeg2AngleStop);
        y1.append(0);
        y1.append(0);
        y2.append(0);
        y2.append(0);
        massZoneLeg1->setData(x1,y1);
        massZoneLeg2->setData(x2,y2);
        //---------------------------
        maxCurrentLeg1Line->start->setCoords(currentFrame->maxCurrentAngle1,0);
        maxCurrentLeg1Line->end->setCoords(currentFrame->maxCurrentAngle1,currentFrame->maxAmperLeg1);
        //--------------------------
        maxCurrentLeg2Line->start->setCoords(currentFrame->maxCurrentAngle2,0);
        maxCurrentLeg2Line->end->setCoords(currentFrame->maxCurrentAngle2,currentFrame->maxAmperLeg2);
        //------------------------------
        middleLineLeg1->start->setCoords(currentFrame->Leg1AngleStart,currentFrame->maxAmperLeg1/2.0);
        middleLineLeg1->end->setCoords(currentFrame->Leg1AngleStop,currentFrame->maxAmperLeg1/2.0);
        //------------------------------
        middleLineLeg2->start->setCoords(currentFrame->Leg2AngleStart,currentFrame->maxAmperLeg2/2.0);
        middleLineLeg2->end->setCoords(currentFrame->Leg2AngleStop,currentFrame->maxAmperLeg2/2.0);
        //-----------------------
        massMiddleLeg1->start->setCoords(currentFrame->massLeg1AngleStart,currentFrame->massMiddleAmperLeg1);
        massMiddleLeg1->end->setCoords(currentFrame->massLeg1AngleStop,currentFrame->massMiddleAmperLeg1);

        massTracerLeg1->position->setCoords( (currentFrame->massLeg1AngleStart+currentFrame->massLeg1AngleStop)/2.0,currentFrame->massMiddleAmperLeg1);

        massMiddleLeg2->start->setCoords(currentFrame->massLeg2AngleStart,currentFrame->massMiddleAmperLeg2);
        massMiddleLeg2->end->setCoords(currentFrame->massLeg2AngleStop,currentFrame->massMiddleAmperLeg2);

        massTracerLeg2->position->setCoords( (currentFrame->massLeg2AngleStart+currentFrame->massLeg2AngleStop)/2.0,currentFrame->massMiddleAmperLeg2);
        //-----------------------
        /*
        plot->xAxis->setRange(xR);
        plot->yAxis->setRange(yR);
        */
        plot->replot();
        //=========================================
        QStringList list = fileName.split("/");
        QString name = list.last();
        infoBlock->setFrame(currentFrameId);
        infoBlock->setFileName(name);
        infoBlock->setScanDate(scanDate);
        infoBlock->setMaxValueData(currentFrame->maxCurrentAngle1,currentFrame->maxCurrentAngle2,currentFrame->maxCurrentX,currentFrame->maxCurrentY,currentFrame->maxAmperLeg1,currentFrame->maxAmperLeg2);
        infoBlock->setWidth(currentFrame->widthLeg1,currentFrame->widthLeg2);
        infoBlock->setCurrent(currentFrame->currentLeg1,currentFrame->currentLeg2,currentFrame->current);
        infoBlock->setMassXY(currentFrame->massX,currentFrame->massY);
        infoBlock->setMassWidth(currentFrame->massWidthLeg1,currentFrame->massWidthLeg2);
        infoBlock->setMassCurrent(currentFrame->currentLeg1Mass,currentFrame->currentLeg2Mass,currentFrame->currentMass,currentFrame->difCurrentLeg1,currentFrame->difCurrentLeg2,currentFrame->difCurrent);
        infoBlock->setPart(currentFrame->part);
        infoBlock->setSection(currentFrame->massLeg1AngleStart,currentFrame->massLeg1AngleStop,currentFrame->massLeg2AngleStart,currentFrame->massLeg2AngleStop,angleToMMKoef);
    }
}

void ProfileData::changePart(double value)
{
    for(int i = 0; i < frames.count();i++){
        frames.at(i)->findMass(value);
    }
    showFrame(currentFrameId);
}

void ProfileData::showMaxCheck(bool on)
{
    maxCurrentLeg1Line->setVisible(on);
    maxCurrentLeg2Line->setVisible(on);
    plot->replot();
}

void ProfileData::showIntervalCheck(bool on)
{
    massZoneLeg1->setVisible(on);
    massZoneLeg2->setVisible(on);
    massMiddleLeg1->setVisible(on);
    massMiddleLeg2->setVisible(on);
    massTracerLeg1->setVisible(on);
    massTracerLeg2->setVisible(on);
    plot->replot();
}


void ProfileData::showMiddleCheck(bool on)
{
    middleLineLeg1->setVisible(on);
    middleLineLeg2->setVisible(on);
    plot->replot();
}

void ProfileData::redrawAll(QColor newColor)
{
    color = newColor;

    mainPen.setColor(color);
    QPen linePen(color);
    linePen.setStyle(Qt::PenStyle::DashLine);
    QPen normalPen(color);
    QColor brushcolor(color);

    brushcolor.setAlpha(100);
    QBrush mainBrush(brushcolor);
    mainBrush.setStyle(Qt::Dense4Pattern);
    ampersGraph->setPen(mainPen);


    QColor c(color);
    c.setAlpha(50);
    QPen shadowPen(c);
    shadowPen.setStyle(Qt::DashLine);

    realAmpersGraph->setPen(shadowPen);

    massZoneLeg1->setBrush(mainBrush);
    massZoneLeg2->setBrush(mainBrush);
    maxCurrentLeg1Line->setPen(linePen);
    maxCurrentLeg2Line->setPen(linePen);
    middleLineLeg1->setPen(linePen);
    middleLineLeg2->setPen(linePen);
    massMiddleLeg1->setPen(normalPen);
    massMiddleLeg2->setPen(normalPen);
    massTracerLeg1->setPen(normalPen);
    massTracerLeg2->setPen(normalPen);
    plot->replot();
}

void ProfileData::showProfileWindow()
{
    if(!profileWindowIsShown && currentFrameId != -1 && !frames.isEmpty()){
        profileWindowIsShown = true;
        ProfileWindow *p = new ProfileWindow(frames.at(currentFrameId)->angles,
                                             frames.at(currentFrameId)->ampers,
                                             frames.at(currentFrameId)->wireLength,
                                             frames.at(currentFrameId)->legLength,frames.at(currentFrameId)->legsAngle,
                                             frames.at(currentFrameId)->zeroAngleIndex,
                                             frames.at(currentFrameId)->showMirror,
                                             frames.at(currentFrameId)->deltaX,
                                             frames.at(currentFrameId)->deltaY,
                                             frames.at(currentFrameId)->maxCurrentX,
                                             frames.at(currentFrameId)->maxCurrentY,
                                             frames.at(currentFrameId)->massLineLeg1StartX,
                                             frames.at(currentFrameId)->massLineLeg1StartY,
                                             frames.at(currentFrameId)->massLineLeg1StopX,
                                             frames.at(currentFrameId)->massLineLeg1StopY,
                                             frames.at(currentFrameId)->massLineLeg2StartX,
                                             frames.at(currentFrameId)->massLineLeg2StartY,
                                             frames.at(currentFrameId)->massLineLeg2StopX,
                                             frames.at(currentFrameId)->massLineLeg2StopY,
                                             scrollArea);
        p->setWindowTitle(fileName);
        p->setAttribute(Qt::WA_DeleteOnClose);
        p->show();
        connect(p,SIGNAL(destroyed(QObject*)),this,SLOT(unseeit()));
    }
}

void ProfileData::unseeit()
{
    profileWindowIsShown = false;
}

void ProfileData::changeLineSize(int value)
{
    mainPen.setWidth(value);
    ampersGraph->setPen(mainPen);
    plot->replot();
}

void ProfileData::changeLineType(Qt::PenStyle type)
{
    mainPen.setStyle(type);
    ampersGraph->setPen(mainPen);
    plot->replot();
}

void ProfileData::denoiseCurrentFrame(int percent)
{
    if(currentFrameId != -1){
        frames.at(currentFrameId)->setNoiseReductionPercent(percent);
        showFrame(currentFrameId);
    }
}


void ProfileData::readData(QString fileName)
{
    if(!QFileInfo(fileName).isFile()){
        QMessageBox::critical(scrollArea,"Ошибка",fileName+"\n Не файл.");
        return;
    }
    if(!fileName.endsWith(".csv")){
        QMessageBox::critical(scrollArea,"Ошибка","Не верное расширение\n Необходимый формат .csv");
        return;
    }
    QFile f(fileName);
    if(!f.open(QIODevice::ReadOnly | QIODevice::Text)){
        QMessageBox::critical(scrollArea,"Ошибка","Файл не открылся.");
        return;
    }
    //---------------------------------------------------------------------------------------
    rawAmpers.clear();
    rawAngles.clear();
    //---------------------------------------------------------------------------------------
    bool findData = false;
    QRegExp dateTampl("Scan Start Time*");
    dateTampl.setPatternSyntax(QRegExp::Wildcard);
    QRegExp dataStartTempl("Scan Angle (degrees),Current (A)*");
    dataStartTempl.setPatternSyntax(QRegExp::Wildcard);
    if(!f.atEnd()){
        while(!findData || !f.atEnd()){
            QString rawLine = f.readLine();
            if(dateTampl.exactMatch(rawLine)){
                rawLine.remove(0,17);
                rawLine.chop(2);
                scanDate = rawLine;
            }
            if(dataStartTempl.exactMatch(rawLine)){
                findData = true;
                break;
            }
        }
        //---------------------------------------------------------------------------------------
        if(findData){
            while(!f.atEnd()){
                QString rawLine = f.readLine();
                QStringList slpittedLine = rawLine.split(",");
                if(slpittedLine.count() == 2){
                    //---------------------------------------------------------------
                    if(slpittedLine[0] != "NaN"){
                        bool good = false;
                        rawAngles.append(slpittedLine[0].toDouble(&good));
                        if(!good){
                            f.close();
                            QMessageBox::critical(scrollArea,"Ошибка","Файл поврежден.");
                            return;
                        }
                    }else{
                        if(!rawAngles.isEmpty()){
                            rawAngles.append(rawAngles.last());
                        }else{
                            rawAngles.append(-14);
                        }
                    }
                    //---------------------------------------------------------------
                    bool good = false;

                    rawAmpers.append(slpittedLine[1].toDouble(&good));
                    if(!good){
                        f.close();
                        QMessageBox::critical(scrollArea,"Ошибка","Файл поврежден.");
                        return;
                    }
                } else{
                    f.close();
                    QMessageBox::critical(scrollArea,"Ошибка","Файл поврежден.");
                    return;
                }
            }
        }else{
            QMessageBox::critical(scrollArea,"Ошибка","Данные не найдены.");
        }
    }
    f.close();

}

void ProfileData::findBorders()
{
    borders.clear();
    if(!rawAngles.isEmpty()){
        QVector<double> smoothAngles = denoiseAngles(rawAngles);
        if(!smoothAngles.isEmpty()){
            for(int i = 1; i < smoothAngles.count()-1; i++){
                if( (smoothAngles.at(i-1) > smoothAngles.at(i) && smoothAngles.at(i+1) > smoothAngles.at(i)) || (smoothAngles.at(i-1) < smoothAngles.at(i) && smoothAngles.at(i+1) < smoothAngles.at(i))){
                    if(!borders.isEmpty()){
                        if(rawAngles.at(borders.last())*rawAngles.at(i) < 0){
                            borders.append(i);
                        }
                    }else{
                        borders.append(i);
                    }
                }
            }
        }
    }
}

void ProfileData::loadFrames(double wireLength,double legLength,double legWidth,double deltaX,double deltaY,double luftAngle,double legsAngle,bool showMirror)
{
    if(!rawAngles.isEmpty() && !rawAmpers.isEmpty() && borders.count() > 1){
        for(int i = 0; i < frames.count(); i++){
            frames.at(i)->deleteLater();
        }
        frames.clear();
        for(int i = 0; i < borders.count()-1; i++){
            Frame::Direction dir;
            if(rawAngles.at(borders.at(i)) > rawAngles.at(borders.at(i+1)))
                dir = Frame::Direction::goDown;
            else
                dir = Frame::Direction::goUp;
            Frame *newFrame = new Frame(&rawAngles,&rawAmpers,dir,borders.at(i),borders.at(i+1),wireLength,legLength,legWidth,legsAngle,deltaX,deltaY,luftAngle,showMirror,this);
            frames.append(newFrame);
        }
    }
}

QVector<double> ProfileData::denoiseAngles(QVector<double> angles)
{
    WaveletSpectrum dwt(angles,WaveletSpectrum::BSPLINE_309);
    dwt.highFilter(0);
    return dwt.toData();
}

void ProfileData:: addToPlot()
{
    mainPen.setColor(color);
    mainPen.setWidth(2);

    QPen linePen(color);
    linePen.setStyle(Qt::PenStyle::DashLine);
    QPen normalPen(color);
    QColor brushcolor(color);


    QColor c(color);
    c.setAlpha(50);
    QPen shadowPen(c);
    shadowPen.setStyle(Qt::DashLine);

    realAmpersGraph = plot->addGraph();
    realAmpersGraph->setPen(shadowPen);
    realAmpersGraph->setLayer("up");


    brushcolor.setAlpha(100);
    QBrush mainBrush(brushcolor);
    mainBrush.setStyle(Qt::Dense4Pattern);
    ampersGraph = plot->addGraph();
    ampersGraph->setPen(mainPen);
    ampersGraph->setLayer("up");



    QColor noColor;
    noColor.setAlpha(0);
    QPen noPen(noColor);
    massZoneLeg1 = plot->addGraph();
    massZoneLeg1->setPen(noPen);
    massZoneLeg1->setChannelFillGraph(ampersGraph);
    massZoneLeg1->setLayer("down");
    massZoneLeg1->setBrush(mainBrush);

    massZoneLeg2 = plot->addGraph();
    massZoneLeg2->setPen(noPen);
    massZoneLeg2->setLayer("down");
    massZoneLeg2->setChannelFillGraph(ampersGraph);
    massZoneLeg2->setBrush(mainBrush);

    massTracerLeg1 = new QCPItemTracer(plot);
    massTracerLeg1->setPen(normalPen);
    massTracerLeg1->setStyle(QCPItemTracer::tsCircle);
    plot->addItem(massTracerLeg1);

    massTracerLeg2 = new QCPItemTracer(plot);
    massTracerLeg2->setPen(normalPen);
    massTracerLeg2->setStyle(QCPItemTracer::tsCircle);
    plot->addItem(massTracerLeg2);

    //----------------------------
    maxCurrentLeg1Line = new QCPItemLine(plot);
    maxCurrentLeg1Line->setLayer("up");
    maxCurrentLeg1Line->setPen(linePen);
    plot->addItem(maxCurrentLeg1Line);
    //----------------------------
    maxCurrentLeg2Line = new QCPItemLine(plot);
    maxCurrentLeg2Line->setLayer("up");
    maxCurrentLeg2Line->setPen(linePen);
    plot->addItem(maxCurrentLeg2Line);
    //----------------------------
    middleLineLeg1 = new QCPItemLine(plot);
    middleLineLeg1->setLayer("up");
    middleLineLeg1->setPen(linePen);
    plot->addItem(middleLineLeg1);
    //---------------------------------
    middleLineLeg2 = new QCPItemLine(plot);
    middleLineLeg2->setLayer("up");
    middleLineLeg2->setPen(linePen);
    plot->addItem(middleLineLeg2);
    //---------------------------------
    massMiddleLeg1 = new QCPItemLine(plot);
    massMiddleLeg1->setLayer("up");
    massMiddleLeg1->setPen(normalPen);
    plot->addItem(massMiddleLeg1);
    //--------------------------
    massMiddleLeg2 = new QCPItemLine(plot);
    massMiddleLeg2->setLayer("up");
    massMiddleLeg2->setPen(normalPen);
    plot->addItem(massMiddleLeg2);


    middleLineLeg1->setVisible(false);
    middleLineLeg2->setVisible(false);
    maxCurrentLeg1Line->setVisible(false);
    maxCurrentLeg2Line->setVisible(false);
    massZoneLeg1->setVisible(false);
    massZoneLeg2->setVisible(false);
    massMiddleLeg1->setVisible(false);
    massMiddleLeg2->setVisible(false);
    massTracerLeg1->setVisible(false);
    massTracerLeg2->setVisible(false);

}

void ProfileData::deleteFromPlot()
{
    plot->removeGraph(ampersGraph);
    plot->removeGraph(massZoneLeg1);
    plot->removeGraph(massZoneLeg2);
    plot->removeItem(maxCurrentLeg1Line);
    plot->removeItem(maxCurrentLeg2Line);
    plot->removeItem(middleLineLeg1);
    plot->removeItem(middleLineLeg2);
    plot->removeItem(massMiddleLeg1);
    plot->removeItem(massMiddleLeg2);
    plot->removeItem(massTracerLeg1);
    plot->removeItem(massTracerLeg2);
    plot->removeGraph(realAmpersGraph);
    plot->replot();
}

void ProfileData::addToScrollArea()
{
    infoBlock = new ProfileInfoBlock(0);




    connect(infoBlock,SIGNAL(removeProfile()),this,SLOT(deletePressed()));
    connect(infoBlock,SIGNAL(showFrame(int)),this,SLOT(showFrame(int)));
    connect(infoBlock,SIGNAL(changePart(double)),this,SLOT(changePart(double)));

    connect(infoBlock,SIGNAL(showMaxCheck(bool)),this,SLOT(showMaxCheck(bool)));
    connect(infoBlock,SIGNAL(showIntervalCheck(bool)),this,SLOT(showIntervalCheck(bool)));
    connect(infoBlock,SIGNAL(showMiddleCheck(bool)),this,SLOT(showMiddleCheck(bool)));
    connect(infoBlock,SIGNAL(redrawAll(QColor)),this,SLOT(redrawAll(QColor)));
    connect(infoBlock,SIGNAL(showProfileWindow()),this,SLOT(showProfileWindow()));
    connect(infoBlock,SIGNAL(changeLineSize(int)),this,SLOT(changeLineSize(int)));
    connect(infoBlock,SIGNAL(changeLineType(Qt::PenStyle)),this,SLOT(changeLineType(Qt::PenStyle)));
    connect(infoBlock,SIGNAL(changeNoiseReduction(int)),this,SLOT(denoiseCurrentFrame(int)));

    scrollArea->widget()->layout()->addWidget(infoBlock);

    currentFrameId = 0;
    infoBlock->setFrameMax(frames.count());
    infoBlock->setFrame(currentFrameId);
    infoBlock->setColor(color);
}

void ProfileData::deleteFromScrollArea()
{
    infoBlock->deleteLater();
}

