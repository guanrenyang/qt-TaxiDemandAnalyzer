#ifndef GRAPHWINDOW_H
#define GRAPHWINDOW_H

#include <QMainWindow>
#include "include.h"
namespace Ui {
class graphWindow;
}

class graphWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit graphWindow(QWidget *parent = nullptr);
    ~graphWindow();

    int computeHourID(QDateTime dateTime);

private:
    Ui::graphWindow *ui;
    QDateTime *allDataStartDay;
    QDateTime *allDataEndDay;
    QVector<int>  startOrderNumber;
    QVector<int>  endOrderNumber;
    QVector<int>  feesrNumber;
    QLineSeries * startLine=new QLineSeries();
    QLineSeries * endLine=new QLineSeries();
    QLineSeries * feesLine=new QLineSeries();
    QChart  * chart;

public slots:
    void addData(QString orderID,QDateTime oneRecordStartDateTime,QDateTime oneRecordEndDateTime,int startGrid,int endGrid,double fees);
    void loadData(StartEnd se);
    void loadGraph();
    void reLoadGraph();
signals:
    void addDataDone();
};

#endif // GRAPHWINDOW_H
