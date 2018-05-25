#include "settingsdialog.h"
#include "ui_settingsdialog.h"

SettingsDialog::SettingsDialog(double wireLength, double legLength, double legWidth,double legAngle, double deltaX, double deltaY, double luftAngle, bool showMirror,double angleToRKoefNumerator, double angleToRKoefDenominator,QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SettingsDialog)
{
    ui->setupUi(this);
    ui->wireLengthBox->setValue(wireLength);
    ui->legLengthBox->setValue(legLength);
    ui->legWidthBox->setValue(legWidth);
    ui->legsAngleBox->setValue(legAngle);
    ui->deltaXBox->setValue(deltaX);
    ui->deltaYBox->setValue(deltaY);
    ui->luftBox->setValue(luftAngle);
    ui->checkBox->setChecked(showMirror);
    ui->angleToRKoefDenominatorBox->setValue(angleToRKoefDenominator);
    ui->angleToRKoefNumeratorBox->setValue(angleToRKoefNumerator);
    ui->currentCalibrationFileNamLabel->setText(MainWindow::calibrationFileName);

    ui->picLabel->setPixmap(QPixmap("settingsPic.png"));
}
SettingsDialog::~SettingsDialog()
{
    delete ui;
}

void SettingsDialog::on_acceptButton_clicked()
{  
    emit settingsUpdate(ui->wireLengthBox->value(),ui->legLengthBox->value(),ui->legWidthBox->value(),ui->legsAngleBox->value(),ui->deltaXBox->value(),ui->deltaYBox->value(),ui->luftBox->value(),ui->checkBox->isChecked(),ui->angleToRKoefNumeratorBox->value(),ui->angleToRKoefDenominatorBox->value());
    this->close();
}


void SettingsDialog::on_saveCalibrationButton_clicked()
{
    QString path = QFileDialog::getSaveFileName(this,"Сохранить калибровку","","Calibration file (*.cb)");
    if(!path.isEmpty()){
        {
            QSettings settings(MainWindow::settingsFileName,QSettings::IniFormat);
            MainWindow::calibrationFileName = path;
            settings.setValue(MainWindow::calibrationFileNameFieldName,MainWindow::calibrationFileName);
            ui->currentCalibrationFileNamLabel->setText(MainWindow::calibrationFileName);
        }
        QSettings settings(MainWindow::calibrationFileName,QSettings::IniFormat);
        settings.setValue(MainWindow::wireLengthFieldName,ui->wireLengthBox->value());
        settings.setValue(MainWindow::legLengthFieldName,ui->legLengthBox->value());
        settings.setValue(MainWindow::legWidthFieldName,ui->legWidthBox->value());
        settings.setValue(MainWindow::deltaXFieldName,ui->deltaXBox->value());
        settings.setValue(MainWindow::deltaYFieldName,ui->deltaYBox->value());
        settings.setValue(MainWindow::luftAngleFieldName,ui->luftBox->value());
        settings.setValue(MainWindow::showMirrorFieldName,ui->checkBox->isChecked());
        settings.setValue(MainWindow::legsAngleFieldName,ui->legsAngleBox->value());
        settings.setValue(MainWindow::angleToRKoefNumeratorFieldName,ui->angleToRKoefNumeratorBox->value());
        settings.setValue(MainWindow::angleToRKoefDenominatorFieldName,ui->angleToRKoefDenominatorBox->value());
    }
}

void SettingsDialog::on_loadCalibrationButton_clicked()
{
    QString path = QFileDialog::getOpenFileName(this,"Открыть калибровку","","Calibration file (*.cb)");
    if(!path.isEmpty()){
        if(QFileInfo(path).isFile()){
            {
                QSettings settings(MainWindow::settingsFileName,QSettings::IniFormat);
                MainWindow::calibrationFileName = path;
                settings.setValue(MainWindow::calibrationFileNameFieldName,MainWindow::calibrationFileName);
                ui->currentCalibrationFileNamLabel->setText(MainWindow::calibrationFileName);
            }

            QSettings settings(path,QSettings::IniFormat);

            ui->wireLengthBox->setValue(settings.value(MainWindow::wireLengthFieldName).toDouble());
            ui->legLengthBox->setValue(settings.value(MainWindow::legLengthFieldName).toDouble());
            ui->legWidthBox->setValue(settings.value(MainWindow::legWidthFieldName).toDouble());
            ui->legsAngleBox->setValue(settings.value(MainWindow::legsAngleFieldName).toDouble());
            ui->deltaXBox->setValue(settings.value(MainWindow::deltaXFieldName).toDouble());
            ui->deltaYBox->setValue(settings.value(MainWindow::deltaYFieldName).toDouble());
            ui->luftBox->setValue(settings.value(MainWindow::luftAngleFieldName).toDouble());
            ui->checkBox->setChecked(settings.value(MainWindow::showMirrorFieldName).toBool());
            ui->angleToRKoefDenominatorBox->setValue(settings.value(MainWindow::angleToRKoefDenominatorFieldName).toDouble());
            ui->angleToRKoefNumeratorBox->setValue(settings.value(MainWindow::angleToRKoefNumeratorFieldName).toDouble());
        }
    }
}
