#include "profileinfoblock.h"
#include "ui_profileinfoblock.h"

ProfileInfoBlock::ProfileInfoBlock(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ProfileInfoBlock)
{
    ui->setupUi(this);
    ui->lineTypeBox->addItem("Сплошная");           //SolidLine
    ui->lineTypeBox->addItem("Штриховая");          //DashLine
    ui->lineTypeBox->addItem("Пунктирная");         //DotLine
    ui->lineTypeBox->addItem("Штрихпунктирная");         //DashDotLine
    ui->lineTypeBox->addItem("Штрихпунктирная с двумя точками");    //DashDotDotLine
}

ProfileInfoBlock::~ProfileInfoBlock()
{
    delete ui; //sfdad
}

void ProfileInfoBlock::setFrame(int id)
{
    ui->frameSlider->setValue(id);
    ui->frameNumberLabel->setText(QString::number(id+1)+"/"+QString::number(ui->frameSlider->maximum()+1));
}

void ProfileInfoBlock::setFrameMax(int value)
{
    ui->frameSlider->setMaximum(value-1);
}

void ProfileInfoBlock::setScanDate(QString date)
{
    ui->scanDateLabel->setText(date);
}

void ProfileInfoBlock::setFileName(QString fileName)
{
    ui->fileNameLabel->setText(fileName);
}

void ProfileInfoBlock::setMaxValueData(double maxAngleLeg1, double maxAngleLeg2, double maxX, double maxY, double maxAmpLeg1, double maxAmpLeg2)
{
    ui->maxAngleLeg1Label->setText(QString::number(maxAngleLeg1));
    ui->maxAngleLeg2Label->setText(QString::number(maxAngleLeg2));
    ui->maxXLabel->setText(QString::number(maxX));
    ui->maxYLabel->setText(QString::number(maxY));
    ui->maxAmpLeg1Label->setText(QString::number(maxAmpLeg1));
    ui->maxAmpLeg2Label->setText(QString::number(maxAmpLeg2));

    ui->commonXLabel->setText(QString::number(qRound(maxX*10)/10.0));
    ui->commonYLabel->setText(QString::number(qRound(maxY*10)/10.0));
}

void ProfileInfoBlock::setWidth(double width1, double width2)
{
    ui->width1Label->setText(QString::number(width1));
    ui->width2Label->setText(QString::number(width2));
    ui->sqLabelMid->setText(QString::number(M_PI*width1*width2/4.0));
    ui->commonWidth1Label->setText(QString::number(qRound(width1*10)/10.0));
    ui->commonWidth2Label->setText(QString::number(qRound(width2*10)/10.0));
}

void ProfileInfoBlock::setCurrent(double currentLeg1, double currentLeg2, double current)
{
    ui->currentLeg1Label->setText(QString::number(currentLeg1));
    ui->currentLeg2Label->setText(QString::number(currentLeg2));
    ui->currentLabel->setText(QString::number(current));
    ui->commonCurrentLabel->setText(QString::number(qRound(current*1000)/1000.0));
}

void ProfileInfoBlock::setMassXY(double massX, double massY)
{
    ui->massXLabel->setText(QString::number(massX));
    ui->massYLabel->setText(QString::number(massY));
}

void ProfileInfoBlock::setMassWidth(double massWidthLeg1, double massWidthLeg2)
{
    ui->massWidthLeg1Label->setText(QString::number(massWidthLeg1));
    ui->massWidthLeg2Label->setText(QString::number(massWidthLeg2));
    ui->sqLabel->setText(QString::number(M_PI*massWidthLeg1*massWidthLeg2/4.0));
}

void ProfileInfoBlock::setMassCurrent(double currentLeg1, double currentLeg2, double current, double difLeg1, double difLeg2, double dif)
{
    ui->massCurrentLeg1Label->setText(QString::number(currentLeg1));
    ui->massCurrentLeg2Label->setText(QString::number(currentLeg2));
    ui->massCurrentLabel->setText(QString::number(current));
    ui->diffLeg1Label->setText(QString::number(difLeg1));
    ui->diffLeg2Label->setText(QString::number(difLeg2));
    ui->diffLabel->setText(QString::number(dif));
}

void ProfileInfoBlock::setSection(double leg1StartAngle, double leg1StopAngle, double leg2StartAngle, double leg2StopAngle, double angleToMMHof)
{
    ui->sectionLeg1StartLabel->setText(QString::number(leg1StartAngle*angleToMMHof));
    ui->sectionLeg1StopLabel->setText(QString::number(leg1StopAngle*angleToMMHof));
    ui->sectionLeg2StartLabel->setText(QString::number(leg2StartAngle*angleToMMHof));
    ui->sectionLeg2StopLabel->setText(QString::number(leg2StopAngle*angleToMMHof));
}

void ProfileInfoBlock::setColor(QColor color)
{
    ui->pushButton->setStyleSheet("background:rgb("+QString::number(color.red())+","+QString::number(color.green())+","+QString::number(color.blue())+")");
}

void ProfileInfoBlock::setPart(double value)
{
    ui->partBox->setValue(value);
}

void ProfileInfoBlock::on_closeButton_clicked()
{
    emit removeProfile();
}

void ProfileInfoBlock::on_frameSlider_valueChanged(int value)
{
    emit showFrame(value);
}

void ProfileInfoBlock::on_partBox_valueChanged(double arg1)
{
    emit changePart(arg1);
}

void ProfileInfoBlock::on_showMaxCheck_clicked(bool checked)
{
    emit showMaxCheck(checked);
}

void ProfileInfoBlock::on_showIntervalCheck_clicked(bool checked)
{
    emit showIntervalCheck(checked);
}

void ProfileInfoBlock::on_showMiddleCheck_clicked(bool checked)
{
    emit showMiddleCheck(checked);
}

void ProfileInfoBlock::on_pushButton_clicked()
{
    QColor color = QColorDialog::getColor();

    ui->pushButton->setStyleSheet("background:rgb("+QString::number(color.red())+","+QString::number(color.green())+","+QString::number(color.blue())+")");
    emit redrawAll(color);
}

void ProfileInfoBlock::on_pushButton_2_clicked()
{
    emit showProfileWindow();
}

void ProfileInfoBlock::on_sizeBox_valueChanged(int arg1)
{
    emit changeLineSize(arg1);
}

void ProfileInfoBlock::on_lineTypeBox_currentIndexChanged(int index)
{
    Qt::PenStyle type;
    type = (Qt::PenStyle)(index + 1);
    emit changeLineType(type);
}

void ProfileInfoBlock::on_noiseReductionSlider_valueChanged(int value)
{
    emit changeNoiseReduction(value);
}
