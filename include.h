#ifndef INCLUDE_H
#define INCLUDE_H

#include<QMainWindow>
#include<QDialog>
#include<QPushButton>
#include<QtDebug>
#include<QThread>
#include<QProgressDialog>
#include<QProgressBar>
#include<QString>
#include<QTime>
#include<QDate>
#include<QMetaType>
#include<QDebug>
#include<QMutex>
#include<QFile>
#include<bits/stdc++.h>
#include<QLabel>
#include<QNetworkRequest>
#include<QNetworkReply>
#include<QNetworkAccessManager>
#include<QJsonObject>
#include<QJsonDocument>
#include<QtCharts>
#include<QChartView>
#include<QGraphicsView>
#include<QLineSeries>
#include<QValueAxis>
#include<QVector>
#include<QTextEdit>
#include <QtWidgets/QApplication>
#include <QtWidgets/QWidget>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QSlider>
#include <QtWidgets/QFontComboBox>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMessageBox>
#include <QtGui/QScreen>
#include <QtGui/QFontDatabase>
#include<QMap>

struct StartEnd
{
    QDate startDate;
    QDate endDate;
    QTime startTime;
    QTime endTime;
};
Q_DECLARE_METATYPE(StartEnd);

#endif // INCLUDE_H
