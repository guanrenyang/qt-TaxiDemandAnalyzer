#ifndef BOOTUPDIALOG_H
#define BOOTUPDIALOG_H

#include"include.h"

namespace Ui {
class BootUpDialog;
}

class BootUpDialog : public QDialog
{
    Q_OBJECT

public:
    explicit BootUpDialog(QWidget *parent = nullptr);
    ~BootUpDialog();

private:
    Ui::BootUpDialog *ui;
};

#endif // BOOTUPDIALOG_H
