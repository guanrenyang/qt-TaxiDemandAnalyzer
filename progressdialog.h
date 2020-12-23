#ifndef PROGRESSDIALOG_H
#define PROGRESSDIALOG_H

#include <QDialog>
#include "include.h"
namespace Ui {
class ProgressDialog;
}

class ProgressDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ProgressDialog(QWidget *parent = nullptr);
    ~ProgressDialog();

private:
    Ui::ProgressDialog *ui;
    QLabel *progress;
public slots:
    void progressMaximum(int);
    void progressValue(int);

};

#endif // PROGRESSDIALOG_H
