#include "loaddatathread.h"

LoadDataThread::LoadDataThread(StartEnd se):
    OriginLongitude(10390840747453),OriginLatitude(3055106155974),DeltaLongitude(3136370509),DeltaLatitude(2697961006)
{
    loadDataMutex.lock();
    this->startEnd=se;
    loadDataMutex.unlock();
}
void LoadDataThread::run()
{
    loadDataMutex.lock();
    int startDayInt=startEnd.startDate.day();
    int endDayInt=startEnd.endDate.day();
    loadDataMutex.unlock();

    loadDataMutex.lock();
    QDateTime startDateTime(startEnd.startDate,startEnd.startTime);
    QDateTime endDateTime(startEnd.endDate,startEnd.endTime);
    loadDataMutex.unlock();

    QString fileNamePrefix=":/new/prefix1/Dataset-CS241-2020/order_201611";
    QString fileNameStem=".csv";


    for(int i=startDayInt;i<=endDayInt;i++)
    {
        for(int j=0;j<=4;j++)
        {


            QString fileName=fileNamePrefix+((i<=9)?("0"+QString::number(i)):QString::number(i))+"_part"+QString::number(j)+fileNameStem;

            QFile testFile(fileName);

            if(!testFile.open(QIODevice::ReadOnly))
            {
                qDebug()<<"Error : Can't load file   "+fileName;
                continue;
            }

            QTextStream * text=new QTextStream(&testFile);

            QStringList lines=text->readAll().split('\n');

            for(int t=1;t<lines.size()-1;t++)
            {
                QStringList oneRecord;//记录一条目录
                oneRecord=lines.at(t).split(',');//每一行的每一个项目

                QString orderID=oneRecord.at(0);

                unsigned int oneRecordStartDateTimeSecs=oneRecord.at(1).toUInt();
                unsigned int oneRecordEndDateTimeSecs=oneRecord.at(2).toUInt();

                QDateTime oneRecordStartDateTime=QDateTime::fromSecsSinceEpoch(oneRecordStartDateTimeSecs);
                QDateTime oneRecordEndDateTime=QDateTime::fromSecsSinceEpoch(oneRecordEndDateTimeSecs);


                double startLongitudeDouble=oneRecord.at(3).toDouble();
                double startLatitudeDouble=oneRecord.at(4).toDouble();
                double endLongitudeDouble=oneRecord.at(5).toDouble();
                double endLatitudeDouble=oneRecord.at(6).toDouble();

                long long startLongitude=startLongitudeDouble*100000000000;
                long long startLatitude=startLatitudeDouble*100000000000;
                long long endLongitude=endLongitudeDouble*100000000000;
                long long endLatitude=endLatitudeDouble*100000000000;
                //判断时间是否吻合

                int startGrid=computeGridNumber(startLongitude,startLatitude);
                int endGrid=computeGridNumber(endLongitude,endLatitude);

                double  fees=oneRecord.at(7).toDouble();


                if(
                        (startDateTime<=oneRecordStartDateTime && oneRecordStartDateTime<=endDateTime)
                        ||(startDateTime<=oneRecordEndDateTime && oneRecordEndDateTime<=endDateTime)
                  )
                emit  emitOneRecord(orderID,oneRecordStartDateTime,oneRecordEndDateTime,startGrid,endGrid,fees);
            }

            int fileNumber=(i-startDayInt)*5 +j+1;
            emit emitFileNumber(fileNumber);//emit progress
        }
    }
    emit loadDone();
}
int LoadDataThread::computeGridNumber( long long longitude, long long latitude)
{
    int x=0;
    int y=0;
    long long longitudeInt=(longitude-OriginLongitude);
    long long latitudeInt=latitude-OriginLatitude;

    while(longitudeInt>=0)
    {
        longitudeInt-=DeltaLongitude;
        x++;
    }
    while(latitudeInt>=0)
    {
        latitudeInt-=DeltaLatitude;
        y++;
    }

    int gridNumber=(y-1)*10+x-1;
    return gridNumber;
}
