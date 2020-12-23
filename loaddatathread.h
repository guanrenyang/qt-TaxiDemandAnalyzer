#ifndef LOADDATATHREAD_H
#define LOADDATATHREAD_H

#include "include.h"

class LoadDataThread :public QThread
{
    Q_OBJECT
    QMutex loadDataMutex;
private:

    const  long long OriginLongitude;
    const  long long OriginLatitude;
    const  long long DeltaLongitude;
    const  long long DeltaLatitude;
    void run() override;
    double prog=0;
    StartEnd startEnd;
    int computeGridNumber( long long longitude, long long latitude);
public:
    LoadDataThread(StartEnd se);
signals:
    void emitFileNumber(int fileNumber);
    void loadDone();
    void emitOneRecord(QString,QDateTime,QDateTime,int,int,double);
    void emitFailedFilename(QString filename);
};


#endif // LOADDATATHREAD_H
