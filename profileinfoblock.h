#ifndef PROFILEINFOBLOCK_H
#define PROFILEINFOBLOCK_H

#include <QWidget>
#include <QtCore>
#include <QColorDialog>

namespace Ui {
class ProfileInfoBlock;
}

class ProfileInfoBlock : public QWidget
{
    Q_OBJECT

public:
    explicit ProfileInfoBlock(QWidget *parent = 0);
    ~ProfileInfoBlock();
    void setFrame(int id);
    void setFrameMax(int value);
    void setScanDate(QString);
    void setFileName(QString);
    void setMaxValueData(double maxAngleLeg1,double maxAngleLeg2,double maxX,double maxY,double maxAmpLeg1,double maxAmpLeg2);
    void setWidth(double width1, double width2);
    void setCurrent(double currentLeg1,double currentLeg2,double current);
    void setMassXY(double massX, double massY);
    void setMassWidth(double massWidthLeg1,double massWidthLeg2);
    void setMassCurrent(double currentLeg1,double currentLeg2, double current, double difLeg1,double difLeg2, double dif);
    void setSection(double leg1StartAngle,double leg1StopAngle,double leg2StartAngle, double leg2StopAngle,double angleToMMHof);
    void setColor(QColor color);
    void setPart(double value);
signals:
    void removeProfile();
    void showFrame(int id);
    void changePart(double value);
    void showMaxCheck(bool);
    void showIntervalCheck(bool);
    void showMiddleCheck(bool);
    void redrawAll(QColor);
    void showProfileWindow();
    void changeLineSize(int value);
    void changeLineType(Qt::PenStyle);
    void changeNoiseReduction(int value);
private slots:


    void on_closeButton_clicked();

    void on_frameSlider_valueChanged(int value);

    void on_partBox_valueChanged(double arg1);

    void on_showMaxCheck_clicked(bool checked);

    void on_showIntervalCheck_clicked(bool checked);

    void on_showMiddleCheck_clicked(bool checked);

    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

    void on_sizeBox_valueChanged(int arg1);

    void on_lineTypeBox_currentIndexChanged(int index);

    void on_noiseReductionSlider_valueChanged(int value);

private:
    Ui::ProfileInfoBlock *ui;
};

#endif // PROFILEINFOBLOCK_H
