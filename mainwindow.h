#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QDebug>
#include <QFileDialog>
#include <QListWidgetItem>
#include <QSettings>
#include "profiledata.h"
#include <directorymonitor.h>
#include "settingsdialog.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    //----------------------------------
    //Settings
    static QString settingsFileName;
    static QString lastPathFieldName;
    static QString wireLengthFieldName;
    static QString legLengthFieldName;
    static QString legWidthFieldName;
    static QString legsAngleFieldName;
    static QString deltaXFieldName;
    static QString deltaYFieldName;
    static QString luftAngleFieldName;
    static QString showMirrorFieldName;
    static QString angleToRKoefNumeratorFieldName;
    static QString angleToRKoefDenominatorFieldName;
private slots:

    void on_selectFolderBt_clicked();

    void on_profilesList_itemActivated(QListWidgetItem *item);

    void on_pathEdit_editingFinished();

    void deleteProfileFromPlot(QString fileName);

    void newFile(QString path);

    void watcher(int,int);
    void on_pushButton_clicked();
    void rescaleAxis(QCPRange r);

    void on_settingsButton_clicked();

    void reloadSettings(double _wireLength, double _legLength, double _legWidth, double _legAngle, double _deltaX, double _deltaY, double _luftAngle, bool _showMirror, double _angleToRKoefNumerator, double _angleToRKoefDenominator);
private:
    bool fromMonitor = false;
    Ui::MainWindow *ui;

    void loadingSettings();
    //----------------------------------
    //global info
    double wireLength = 169;
    double legLength = 50;
    double legWidth = 0.5;
    double deltaX = -190;
    double deltaY = 0;
    double luftAngle = 0.4;
    double angleToRKoefNumerator = 15;
    double angleToRKoefDenominator = 6.8;
    double angleToRKoef = angleToRKoefNumerator/angleToRKoefDenominator;
    double legsAngle = 103.6;
    bool showMirror = true;
    //----------------------------------
    void scanProfiles(QString folderPath);
    void addProfileToPlot(QString fileName);
    QList<ProfileData*> profiles;
    void initPlot();
    DirectoryMonitor *monitor = nullptr;
};

#endif // MAINWINDOW_H
