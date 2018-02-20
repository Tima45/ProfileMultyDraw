#include "mainwindow.h"
#include "ui_mainwindow.h"


QString MainWindow::settingsFileName = "settings.ini";
QString MainWindow::lastPathFieldName = "LastPath";
QString MainWindow::wireLengthFieldName = "WireLength_mm";
QString MainWindow::legLengthFieldName = "LegLength_mm";
QString MainWindow::legWidthFieldName = "LegWidth_mm";
QString MainWindow::legsAngleFieldName = "LegsAngle_c";
QString MainWindow::deltaXFieldName = "DeltaX_mm";
QString MainWindow::deltaYFieldName = "DeltaY_mm";
QString MainWindow::luftAngleFieldName = "LuftAngle_c";
QString MainWindow::showMirrorFieldName = "ShowMirror";
QString MainWindow::angleToRKoefNumeratorFieldName = "mmAxisCalibNumerator_mm";
QString MainWindow::angleToRKoefDenominatorFieldName = "mmAxisCalibDenominator_c";


//test
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    QWidget *widget = new QWidget(ui->scrollArea);

    QBoxLayout *lay = new QBoxLayout(QBoxLayout::TopToBottom,widget);

    widget->setLayout(lay);

    ui->scrollArea->setWidget(widget);

    connect(ui->scrollArea->verticalScrollBar(),SIGNAL(rangeChanged(int,int)),this,SLOT(watcher(int,int)));
    loadingSettings();
    initPlot();
}

MainWindow::~MainWindow()
{
    QSettings settings(settingsFileName,QSettings::IniFormat);
    settings.setValue(lastPathFieldName,ui->pathEdit->text());

    ui->profilesList->clear();
    for(int i = 0; i < profiles.count(); i++){
        profiles.at(i)->deleteLater();
    }
    profiles.clear();
    delete ui;
}

void MainWindow::topIt()
{
    raise();
    activateWindow();
    showNormal();
    setWindowState(Qt::WindowActive);
}
void MainWindow::on_selectFolderBt_clicked()
{
    QString folderPath = QFileDialog::getExistingDirectory(this,"Выбрать папку",ui->pathEdit->text(),0);
    ui->pathEdit->setText(folderPath);
    scanProfiles(folderPath);
}

void MainWindow::scanProfiles(QString folderPath)
{

    ui->profilesList->clear();
    for(int i = 0; i < profiles.count(); i++){
        profiles.at(i)->deleteLater();
    }
    profiles.clear();
    if(monitor != nullptr){
        monitor->stop();
        delete monitor;
    }
    monitor = new DirectoryMonitor(folderPath,1000,".csv",false);
    connect(monitor,SIGNAL(fileChanged(QString)),this,SLOT(newFile(QString)));
    connect(monitor,SIGNAL(fileChanged(QString)),this,SLOT(topIt()));
    monitor->start();

    QDir dir(folderPath);
    QFileInfoList fileInfoList = dir.entryInfoList();
    for(int i = 0; i < fileInfoList.count(); i++){
        if(fileInfoList.at(i).fileName().endsWith(".csv")){
            ui->profilesList->addItem(fileInfoList.at(i).fileName());
        }
    }
}

void MainWindow::addProfileToPlot(QString fileName)
{  
    bool find = false;
    for(int i = 0; i < profiles.count(); i++){
        if(profiles.at(i)->fileName == fileName){
            find = true;
            break;
        }
    }
    if(!find){
        QColor randomColor(qrand()%256,qrand()%256,qrand()%256);
        ProfileData *newProfile = new ProfileData(fileName,ui->plot,ui->scrollArea,randomColor,wireLength,legLength,legWidth,legsAngle,deltaX,deltaY,luftAngle,showMirror,angleToRKoef,0);
        if(!newProfile->errors){
            connect(newProfile,SIGNAL(wantToBeDeleted(QString)),this,SLOT(deleteProfileFromPlot(QString)));
            profiles.append(newProfile);
            ui->plot->rescaleAxes();
            ui->plot->replot();
        }else{

        }
    }
}

void MainWindow::initPlot()
{
    ui->plot->setInteraction(QCP::iRangeDrag, true);
    ui->plot->setInteraction(QCP::iRangeZoom, true);
    ui->plot->axisRect()->setRangeDrag(Qt::Horizontal);
    ui->plot->axisRect()->setRangeZoom(Qt::Horizontal);
    ui->plot->xAxis->setLabel("Угол");
    ui->plot->yAxis->setLabel("Ток(А)");
    ui->plot->xAxis2->setLabel("Мм");
    ui->plot->addLayer("down");
    ui->plot->addLayer("up");
    ui->plot->xAxis2->setVisible(true);
    ui->plot->xAxis2->setRange(QCPRange(-angleToRKoef*13.0,angleToRKoef*13.0));
    connect(ui->plot->xAxis,SIGNAL(rangeChanged(QCPRange)),this,SLOT(rescaleAxis(QCPRange)));
}

void MainWindow::on_profilesList_itemActivated(QListWidgetItem *item)
{
    addProfileToPlot(ui->pathEdit->text()+"/"+item->text());
}

void MainWindow::on_pathEdit_editingFinished()
{
    scanProfiles(ui->pathEdit->text());
}

void MainWindow::deleteProfileFromPlot(QString fileName)
{
    int idToDelte = -1;
    for(int i = 0; i < profiles.count(); i++){
        if(profiles.at(i)->fileName == fileName){
            idToDelte = i;
        }
    }
    if(idToDelte != -1){
        //---
        profiles.at(idToDelte)->deleteLater();
        profiles.removeAt(idToDelte);
    }
}

void MainWindow::newFile(QString path)
{
    ui->profilesList->clear();
    QDir dir(ui->pathEdit->text());
    QFileInfoList fileInfoList = dir.entryInfoList();
    for(int i = 0; i < fileInfoList.count(); i++){
        if(fileInfoList.at(i).fileName().endsWith(".csv")){
            ui->profilesList->addItem(fileInfoList.at(i).fileName());
        }
    }
    addProfileToPlot(path);
    fromMonitor = true;
}

void MainWindow::watcher(int min, int max)
{
    if(fromMonitor){
        fromMonitor = false;
        ui->scrollArea->verticalScrollBar()->setValue(max);
        Q_UNUSED(min);
    }
}

void MainWindow::loadingSettings()
{
    if(QFileInfo(settingsFileName).isFile()){
        QSettings settings(settingsFileName,QSettings::IniFormat);      
        ui->pathEdit->setText(settings.value(lastPathFieldName).toString());
        wireLength = settings.value(wireLengthFieldName).toDouble();
        legLength = settings.value(legLengthFieldName).toDouble();
        legWidth = settings.value(legWidthFieldName).toDouble();
        deltaX = settings.value(deltaXFieldName).toDouble();
        deltaY = settings.value(deltaYFieldName).toDouble();
        luftAngle = settings.value(luftAngleFieldName).toDouble();
        showMirror = settings.value(showMirrorFieldName).toBool();
        legsAngle = settings.value(legsAngleFieldName).toDouble();
        angleToRKoefNumerator = settings.value(angleToRKoefNumeratorFieldName).toDouble();
        angleToRKoefDenominator  = settings.value(angleToRKoefDenominatorFieldName).toDouble();
        angleToRKoef = angleToRKoefNumerator/angleToRKoefDenominator;
    }else{
        QMessageBox::warning(this,"Внимание","Файл настрок не найден. Значения дефолтные.");
        QSettings settings(settingsFileName,QSettings::IniFormat);
        settings.setValue(lastPathFieldName,"");
        settings.setValue(wireLengthFieldName,wireLength);
        settings.setValue(legLengthFieldName,legLength);
        settings.setValue(legWidthFieldName,legWidth);
        settings.setValue(deltaXFieldName,deltaX);
        settings.setValue(deltaYFieldName,deltaY);
        settings.setValue(luftAngleFieldName,luftAngle);
        settings.setValue(showMirrorFieldName,showMirror);
        settings.setValue(legsAngleFieldName,legsAngle);
        settings.setValue(angleToRKoefNumeratorFieldName,angleToRKoefNumerator);
        settings.setValue(angleToRKoefDenominatorFieldName,angleToRKoefDenominator);
    }
}

void MainWindow::on_pushButton_clicked()
{
    QString s = QFileDialog::getSaveFileName(this,"Сохранить скриншот",ui->pathEdit->text(),"png");
    ui->plot->grab().save(s+".png","png");
}

void MainWindow::rescaleAxis(QCPRange r)
{
    ui->plot->xAxis2->setRange(QCPRange(angleToRKoef * r.lower,angleToRKoef * r.upper));
}

void MainWindow::on_settingsButton_clicked()
{  
    SettingsDialog *dialog = new SettingsDialog(wireLength,legLength,legWidth,legsAngle,deltaX,deltaY,luftAngle,showMirror,angleToRKoefNumerator,angleToRKoefDenominator,this);
    connect(dialog,SIGNAL(settingsUpdate(double,double,double,double,double,double,double,bool,double,double)),this,SLOT(reloadSettings(double,double,double,double,double,double,double,bool,double,double)));
    dialog->exec();
}

void MainWindow::reloadSettings(double _wireLength,double _legLength,double _legWidth,double _legAngle, double _deltaX, double _deltaY, double _luftAngle, bool _showMirror,double _angleToRKoefNumerator, double _angleToRKoefDenominator)
{
    ui->profilesList->clear();
    for(int i = 0; i < profiles.count(); i++){
        profiles.at(i)->deleteLater();
    }
    profiles.clear();
    if(monitor != nullptr){
        monitor->stop();
        delete monitor;
        monitor = nullptr;
    }

    wireLength = _wireLength;
    legLength = _legLength;
    legWidth = _legWidth;
    legsAngle = _legAngle;
    deltaX = _deltaX;
    deltaY = _deltaY;
    luftAngle = _luftAngle;
    showMirror = _showMirror;
    angleToRKoefNumerator = _angleToRKoefNumerator;
    angleToRKoefDenominator = _angleToRKoefDenominator;
    angleToRKoef = angleToRKoefNumerator/angleToRKoefDenominator;
}
