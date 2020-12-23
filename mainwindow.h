#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "include.h"
#include "selectdialog.h"
#include "loaddatathread.h"
#include "graphwindow.h"
#include "progressdialog.h"
QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void Menu();
    void OpenSelectWindow();

private:

    Ui::MainWindow *ui;
    LoadDataThread *loadDataThread;
    Dialog *selectWindow;
    graphWindow *graphWindow;
    ProgressDialog *progressDialog;
    QWidget *widget;//The widget of 3D graph


    QVector<QVector<int>> gridStartPerDay;
    QVector<QVector<int>> gridEndPerDay;
    QVector<QVector<double>> gridFeesPerDay;

    QDateTime *startDateTime;
    QDateTime *endDateTime;
    qint64 allHours;

    QUrl address2Coordinate;
    QUrl coordinate2PathWalk;
    QUrl coordinate2PathRide;
    QUrl coordinate2PathDrive;

    QString startPosition;
    QString endPosition;

signals:
    void emitDateTime(QList<QString>);
    void loadGridDataDone();
    void finished(QNetworkReply *);

public slots:
    void InitializeGridData(StartEnd startEnd);
    void LoadGridData(QString orderID,QDateTime oneRecordStartDateTime,QDateTime oneRecordEndDateTime,int startGrid,int endGrid,double fees);
    void drawProgressBar();
    void LoadData(StartEnd se);
    void ShowGraph();
    void requestNavigation();
    void releaseButton();
};
#endif // MAINWINDOW_H
