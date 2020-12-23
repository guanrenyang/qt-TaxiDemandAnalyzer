#include "graphwindow.h"
#include "ui_graphwindow.h"

graphWindow::graphWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::graphWindow)
{
    ui->setupUi(this);
    this->setWindowTitle("Data over time");
    connect(ui->draw,SIGNAL(clicked()),this,SLOT(reLoadGraph()));
}

graphWindow::~graphWindow()
{
    delete ui;
}

void graphWindow::loadData(StartEnd se)
{
    allDataStartDay=new QDateTime(se.startDate,se.startTime);
    allDataEndDay=new QDateTime(se.endDate,se.endTime);
    int dayTimeSpan=allDataEndDay->date().day()-allDataStartDay->date().day()+1;

    ui->endTime->setMaximumDateTime(*allDataEndDay);
    ui->startTime->setMinimumDateTime(*allDataStartDay);
    ui->startTime->setDateTime(*allDataStartDay);
    ui->endTime->setDateTime(*allDataEndDay);


    startOrderNumber.resize(dayTimeSpan*24+5);
    endOrderNumber.resize(dayTimeSpan*24+5);
    feesrNumber.resize(dayTimeSpan*24+5);

}
void graphWindow::addData(QString orderID,QDateTime oneRecordStartDateTime,QDateTime oneRecordEndDateTime,int startGrid,int endGrid,double fees)
{
    int startHourID= computeHourID(oneRecordStartDateTime);
    int endHourID=computeHourID(oneRecordEndDateTime);

    if(startHourID>=startOrderNumber.size())
    {
        return;
    }
    if(endHourID>=endOrderNumber.size())
    {
        return;
    }
    startOrderNumber[startHourID]++;
    feesrNumber[startHourID]+=fees;
    endOrderNumber[endHourID]++;
}
void graphWindow::loadGraph()
{
    startLine=new QLineSeries(this);
    feesLine=new QLineSeries(this);
    chart=new QChart();
    for(int i=0;i<startOrderNumber.size()-5;i++)
    {

        startLine->append(i,startOrderNumber[i]);
        feesLine->append(i,feesrNumber[i]);
    }
    for(int i=0;i<endOrderNumber.size()-5;i++)
    {
        endLine->append(i,endOrderNumber[i]);
    }

    startLine->setName("Number of departure orders over time");
//    endLine->setName("Number of departure orders over time");
    feesLine->setName("Total fees of departure orders over time");

    chart->addSeries(startLine);
//    chart->addSeries(endLine);
    chart->addSeries(feesLine);

    QValueAxis *axisX = new QValueAxis;
    QValueAxis *axisY = new QValueAxis;
    axisX->setRange(0, startOrderNumber.size());
    axisX->setTitleText("time(h)");
    axisX->setLabelFormat("%d");

    axisY->setRange(0,3000);

    chart->addAxis(axisX, Qt::AlignBottom);
    chart->addAxis(axisY, Qt::AlignLeft);

    this->show();
    ui->chartView->setChart(chart);
    ui->chartView->show();

}
int graphWindow::computeHourID(QDateTime dateTime)
{

    int hourID=allDataStartDay->secsTo(dateTime)/3600;
    return hourID;
}
void graphWindow::reLoadGraph()
{
    QDateTime startDraw=ui->startTime->dateTime();
    QDateTime endDraw=ui->endTime->dateTime();

    startLine=new QLineSeries(this);
    feesLine=new QLineSeries(this);
    chart=new QChart();

    int startHourID=computeHourID(startDraw);
    int endHourID=computeHourID(endDraw);

    for(int i=startHourID;i<=endHourID;i++)
    {
        startLine->append(i,startOrderNumber[i]);
        feesLine->append(i,feesrNumber[i]);
    }
    for(int i=startHourID;i<endHourID;i++)
    {
        endLine->append(i,endOrderNumber[i]);
    }

    startLine->setName("Number of Departure orders over time");
//    endLine->setName("Number of Arrival orders over time");
    feesLine->setName("Total fees of departure orders over time");

    chart->addSeries(startLine);
//    chart->addSeries(endLine);
    chart->addSeries(feesLine);

    QValueAxis *axisX = new QValueAxis;
    QValueAxis *axisY = new QValueAxis;
    axisX->setRange(0, startOrderNumber.size());//设置坐标轴范围
    axisX->setTitleText("time(h)");//标题
    axisX->setLabelFormat("%d"); //标签格式：每个单位保留1位小数

    axisY->setRange(0,1000);

    chart->addAxis(axisX, Qt::AlignBottom);
    chart->addAxis(axisY, Qt::AlignLeft);

    this->show();
    ui->chartView->setChart(chart);
    ui->chartView->show();



}
