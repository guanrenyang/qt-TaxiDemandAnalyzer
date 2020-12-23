#ifndef DIALOG_H
#define DIALOG_H

#include "include.h"


namespace Ui {
class Dialog;
}

class Dialog : public QDialog
{
    Q_OBJECT

public:
    explicit Dialog(QWidget *parent = nullptr);
    ~Dialog();

private:
    Ui::Dialog *ui;
signals:
    void emitDateTime(StartEnd se);
    void emitAllFilesNumber(int allFilesNumber);
};

#endif // DIALOG_H
