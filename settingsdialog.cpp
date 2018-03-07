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
    ui->picLabel->setPixmap(QPixmap("settingsPic.png"));
}
SettingsDialog::~SettingsDialog()
{
    delete ui;
    this->deleteLater();
}

void SettingsDialog::on_acceptButton_clicked()
{
    QSettings settings(MainWindow::settingsFileName,QSettings::IniFormat);
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
    emit settingsUpdate(ui->wireLengthBox->value(),ui->legLengthBox->value(),ui->legWidthBox->value(),ui->legsAngleBox->value(),ui->deltaXBox->value(),ui->deltaYBox->value(),ui->luftBox->value(),ui->checkBox->isChecked(),ui->angleToRKoefNumeratorBox->value(),ui->angleToRKoefDenominatorBox->value());
    this->close();
}

