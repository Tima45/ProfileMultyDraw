#ifndef SETTINGSDIALOG_H
#define SETTINGSDIALOG_H

#include <QDialog>
#include <QSettings>
#include <QFileDialog>
#include "mainwindow.h"
namespace Ui {
class SettingsDialog;
}

class SettingsDialog : public QDialog
{
    Q_OBJECT
signals:
    void settingsUpdate(double wireLength,double legLength,double legWidth,double legAngle, double deltaX, double deltaY, double luftAngle, bool showMirror,double angleToRKoefNumerator, double angleToRKoefDenominator);
public:
    explicit SettingsDialog(double wireLength,double legLength,double legWidth,double legAngle, double deltaX, double deltaY, double luftAngle, bool showMirror,double angleToRKoefNumerator, double angleToRKoefDenominator,QWidget *parent = 0);

    ~SettingsDialog();

private slots:
    void on_acceptButton_clicked();

    void on_saveCalibrationButton_clicked();

    void on_loadCalibrationButton_clicked();

private:
    Ui::SettingsDialog *ui;

};

#endif // SETTINGSDIALOG_H
