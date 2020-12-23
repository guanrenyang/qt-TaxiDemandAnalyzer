#include "selectdialog.h"
#include "ui_selectdialog.h"

Dialog::Dialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog)
{
    ui->setupUi(this);
    connect(ui->time,&QCheckBox::stateChanged,[=](){
        if(ui->time->isChecked())
        {
            ui->startTime->setEnabled(true);
            ui->endTime->setEnabled((true));
        }
        else
        {
            ui->startTime->setEnabled(false);
            ui->endTime->setEnabled((false));
        }
    });
    connect(ui->date,&QCheckBox::stateChanged,[=](){
        if(ui->date->isChecked())
        {
            ui->startDate->setEnabled(true);
            ui->endDate->setEnabled((true));
        }
        else
        {
            ui->startDate->setEnabled(false);
            ui->endDate->setEnabled((false));
        }
    });
    this->setWindowTitle("Selected Data");
    connect(ui->done,&QPushButton::clicked,[=](){
        QVector<int> gridNumber;

        StartEnd se;
        se.startDate=ui->startDate->date();
        se.endDate=ui->endDate->date();
        se.startTime=ui->startTime->time();
        se.endTime=ui->endTime->time();
        emit emitDateTime(se);
        emit emitAllFilesNumber((se.endDate.day()-se.startDate.day()+1)*5);
    });
    connect(this,SIGNAL(emitDateTime(StartEnd)),this,SLOT(close()));
}

Dialog::~Dialog()
{
    delete ui;
}
