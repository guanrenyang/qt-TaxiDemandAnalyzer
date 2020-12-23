#include "progressdialog.h"
#include "ui_progressdialog.h"

ProgressDialog::ProgressDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ProgressDialog)
{
    ui->setupUi(this);
    progress=new QLabel(ui->progressBar);
    this->setWindowTitle("Loading Data");
}

ProgressDialog::~ProgressDialog()
{
    delete ui;
}
void ProgressDialog::progressMaximum(int max)
{
    ui->progressBar->setMaximum(max);
}
void ProgressDialog::progressValue(int value)
{
    ui->progressBar->setValue(value);
}

