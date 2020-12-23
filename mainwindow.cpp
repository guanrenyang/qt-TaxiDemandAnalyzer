#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "bootupdialog.h"
#include "selectdialog.h"
#include "graphmodifier.h"
#include "graphwindow.h"
#include<QString>
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , address2Coordinate(QUrl("https://restapi.amap.com/v3/geocode/geo?"))
    , coordinate2PathWalk((QUrl("https://restapi.amap.com/v3/direction/walking?")))
    , coordinate2PathRide(QUrl("https://restapi.amap.com/v3/direction/bicycling?"))
    , coordinate2PathDrive((QUrl("https://restapi.amap.com/v3/direction/driving?")))
{   

    progressDialog=new ProgressDialog(this);

    BootUpDialog bootUpDialog(this);
    bootUpDialog.exec();

    ui->setupUi(this);
    connect(ui->draw3DGraph,&QPushButton::clicked,[=](){
        this->ShowGraph();
    });
    this->setWindowTitle("Taxi Damand Analysis");
    Menu();
    connect(ui->navigation,SIGNAL(clicked()),this,SLOT(requestNavigation()));
    graphWindow = new class graphWindow(this);
    connect(ui->chartButton,SIGNAL(clicked()),graphWindow,SLOT(loadGraph()));

}

MainWindow::~MainWindow()
{
    delete ui;
}
void MainWindow::Menu()
{
    QMenuBar * menuBar = new QMenuBar(this);

    QMenu * menu = new QMenu("File", this);

    QAction * loadData = new QAction(QIcon(), "Load Data", menu);//此处QIcon可给Action一个图标

    connect(loadData,&QAction::triggered,this,&MainWindow::OpenSelectWindow);

    menu->addAction(loadData);

    menuBar->addMenu(menu);

    this->setMenuBar(menuBar);
}

void MainWindow::OpenSelectWindow()
{
    selectWindow=new Dialog(this);

    connect(selectWindow,SIGNAL(emitDateTime(StartEnd)),this,SLOT(LoadData(StartEnd)));
    connect(selectWindow,SIGNAL(emitDateTime(StartEnd)),this,SLOT(InitializeGridData(StartEnd)));
    connect(selectWindow,SIGNAL(emitDateTime(StartEnd)),graphWindow,SLOT(loadData(StartEnd)));
    selectWindow->exec();
}
void MainWindow::LoadData(StartEnd se)
{
    loadDataThread=new LoadDataThread(se);

    drawProgressBar();
    loadDataThread->start();

    connect(loadDataThread,SIGNAL(loadDone()),this,SLOT(releaseButton()));

}
void MainWindow::drawProgressBar()
{

    QLabel *loadStatus=new QLabel();
    loadStatus->setText("Loading ...");

    connect(selectWindow,SIGNAL(emitAllFilesNumber(int)),progressDialog,SLOT(progressMaximum(int)));
    connect(loadDataThread,SIGNAL(emitFileNumber(int)),progressDialog,SLOT(progressValue(int)),Qt::QueuedConnection);
    connect(loadDataThread,SIGNAL(emitOneRecord(QString,QDateTime,QDateTime,int,int,double)),this,SLOT(LoadGridData(QString,QDateTime,QDateTime,int,int,double)),Qt::QueuedConnection);
    connect(loadDataThread,SIGNAL(emitOneRecord(QString,QDateTime,QDateTime,int,int,double)),graphWindow,SLOT(addData(QString,QDateTime,QDateTime,int,int,double)),Qt::QueuedConnection);
    connect(loadDataThread,SIGNAL(loadDone()),progressDialog,SLOT(close()));

    progressDialog->show();
    progressDialog->raise();
    progressDialog->activateWindow();
}
void MainWindow::InitializeGridData(StartEnd startEnd)
{

    startDateTime=new QDateTime(startEnd.startDate,startEnd.startTime);
    endDateTime=new QDateTime(startEnd.endDate,startEnd.endTime);
    allHours=startDateTime->secsTo(*endDateTime)/3600+1;

    gridStartPerDay.resize(100);
    gridEndPerDay.resize(100);
    gridFeesPerDay.resize(100);

    for(int i=0;i<gridStartPerDay.size();i++)
    {
        gridStartPerDay[i].resize(allHours);
    }
    for(int i=0;i<gridEndPerDay.size();i++)
    {
        gridEndPerDay[i].resize(allHours);
    }
    for(int i=0;i<gridFeesPerDay.size();i++)
    {
        gridFeesPerDay[i].resize(allHours);
    }
}
void MainWindow::LoadGridData(QString orderID,QDateTime oneRecordStartDateTime,QDateTime oneRecordEndDateTime,int startGrid,int endGrid,double fees)
{
    int oneRecordStartDay = oneRecordStartDateTime.date().day();
    int oneRecordEndDay = oneRecordEndDateTime.date().day();
    if(
            0<=startGrid&&startGrid<=99 &&
            0<=endGrid&&endGrid<=99
      )
    {

        gridStartPerDay[startGrid][oneRecordStartDay]+=1;
        gridFeesPerDay[startGrid][oneRecordStartDay]+=fees;

        gridEndPerDay[endGrid][oneRecordEndDay]+=1;
        gridFeesPerDay[endGrid][oneRecordEndDay]+=fees;
    }

}
void MainWindow::requestNavigation()
{
    QNetworkAccessManager* startNetwork = new QNetworkAccessManager(this);
    QNetworkAccessManager* endNetwork = new QNetworkAccessManager(this);

    QUrl startUrl=address2Coordinate;
    QUrl endUrl=address2Coordinate;

    QNetworkRequest startRequest;
    QNetworkRequest endRequest;

    QUrlQuery startQuery;
    QUrlQuery endQuery;

    startQuery.addQueryItem("output","JSON");
    startQuery.addQueryItem("key","bed12652c6259f360dd2ce59d10bba78");
    endQuery.addQueryItem("output","JSON");
    endQuery.addQueryItem("key","bed12652c6259f360dd2ce59d10bba78");

    QString startAddress=ui->startAddress->toPlainText();
    QString endAddress=ui->endAddress->toPlainText();

    startQuery.addQueryItem("address",startAddress);
    endQuery.addQueryItem("address",endAddress);

    startUrl.setQuery(startQuery);
    endUrl.setQuery(endQuery);

    startRequest.setUrl(startUrl);
    endRequest.setUrl(endUrl);



    connect(startNetwork,&QNetworkAccessManager::finished,[=](QNetworkReply * reply){

        QVariant statusCode = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute);
        QVariant reason = reply->attribute(QNetworkRequest::HttpReasonPhraseAttribute).toString();
        QNetworkReply::NetworkError err = reply->error();
        if(statusCode.isValid()&&reason.isValid()&&err==QNetworkReply::NoError)
        {
            QByteArray allData= reply->readAll();
            QJsonDocument document=QJsonDocument::fromJson(allData);
            if(document.isObject())
            {
                QJsonObject object=document.object();
                if(object.contains("geocodes"))
                {
                    QJsonArray array=object.take("geocodes").toArray();
                    QVariantList variantList = array.toVariantList();
                    if(!variantList.isEmpty())
                    {
                        QVariant variant=variantList[0];
                        QMap<QString,QVariant> map=variant.toMap();
                        startPosition=map.value("location").toString();
                    }
                    else
                    {
                        ui->distance->setText("Please specity your address");
                        ui->estimateTime->setText("Please specity your address");
                        ui->taxiCost->setText("Please specity your address");
                    }
                }
            }

        }
        Sleep(50);
    });
    startNetwork->get(startRequest);


    connect(endNetwork,&QNetworkAccessManager::finished,[=](QNetworkReply * reply){


        QVariant statusCode = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute);
        QVariant reason = reply->attribute(QNetworkRequest::HttpReasonPhraseAttribute).toString();
        QNetworkReply::NetworkError err = reply->error();
        if(statusCode.isValid()&&reason.isValid()&&err==QNetworkReply::NoError)
        {
            QByteArray allData= reply->readAll();
            QJsonDocument document=QJsonDocument::fromJson(allData);
            QJsonObject object=document.object();

            QJsonValue value=object.take("geocodes");
            QJsonArray array=value.toArray();
            QVariantList variantList = array.toVariantList();
            if(!variantList.isEmpty())
            {
                QVariant variant=variantList[0];
                QMap<QString,QVariant> map=variant.toMap();
                endPosition=map.value("location").toString();



                QNetworkAccessManager* driveManager = new QNetworkAccessManager(this);

                QUrl pathUrl=coordinate2PathDrive;

                QNetworkRequest pathRequest;

                QUrlQuery pathQuery;

                pathQuery.addQueryItem("output","JSON");
                pathQuery.addQueryItem("key","bed12652c6259f360dd2ce59d10bba78");
                pathQuery.addQueryItem("origin",startPosition);
                pathQuery.addQueryItem("destination",endPosition);

                pathUrl.setQuery(pathQuery);

                pathRequest.setUrl(pathUrl);

                connect(driveManager,&QNetworkAccessManager::finished,[=](QNetworkReply * reply){
                    QByteArray byteArray=reply->readAll();
                    QJsonDocument jsonDocument=QJsonDocument::fromJson(byteArray);
                    QJsonObject jsonObject=jsonDocument.object();
                    QJsonObject route=jsonObject.take("route").toObject();
                    QString taxicost = route.take("taxi_cost").toString();
                    ui->taxiCost->setText(taxicost+"  RMB");

                    QJsonArray pathsArray=route.take("paths").toArray();
                    if(!pathsArray.toVariantList().isEmpty())
                    {
                        QVariant variant = pathsArray.toVariantList().first();
                        QMap<QString,QVariant> paths=variant.toMap();

                        double distanceDouble=paths["distance"].toDouble();
                        QString distance;
                        if(distanceDouble<1000)
                            distance=QString::number(distanceDouble,'d',1)+" m";
                        else
                            distance=QString::number(distanceDouble/1000,'d',1)+" km";
                        ui->distance->setText(distance);

                        int duration=paths["duration"].toInt();

                        int hours=duration/3600;
                        duration%=3600;
                        int minutes=duration/60;
                        duration%=60;
                        int seconds=duration;
                        if(hours==0)
                            ui->estimateTime->setText(QString::number(minutes)+" minutes"+QString::number(seconds)+" seconds");
                        else
                            ui->estimateTime->setText(QString::number(hours)+" hours"+QString::number(minutes)+" minutes"+QString::number(seconds)+" seconds");
                    }

                });
                driveManager->get(pathRequest);
            }
            else
            {
                ui->distance->setText("Please specity your address");
                ui->estimateTime->setText("Please specity your address");
                ui->taxiCost->setText("Please specity your address");
            }

        }
    });
    endNetwork->get(endRequest);

}

void MainWindow::ShowGraph()
{
    Q3DBars *widgetgraph = new Q3DBars();
    QWidget *container = QWidget::createWindowContainer(widgetgraph);

    if (!widgetgraph->hasContext()) {
        QMessageBox msgBox;
        msgBox.setText("Couldn't initialize the OpenGL context.");
        msgBox.exec();
        qDebug()<< -1;
    }

    QSize screenSize = widgetgraph->screen()->size();
    container->setMinimumSize(QSize(screenSize.width() / 2, screenSize.height() / 1.5));
    container->setMaximumSize(screenSize);
    container->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    container->setFocusPolicy(Qt::StrongFocus);

    widget = new QWidget;
    QHBoxLayout *hLayout = new QHBoxLayout(widget);
    QVBoxLayout *vLayout = new QVBoxLayout();
    hLayout->addWidget(container, 1);
    hLayout->addLayout(vLayout);


    widget->setWindowTitle(QStringLiteral("Didi Taxi Damand Analyzer"));

    QComboBox *barStyleList = new QComboBox(widget);
    barStyleList->addItem(QStringLiteral("Bar"), int(QAbstract3DSeries::MeshBar));
    barStyleList->addItem(QStringLiteral("Pyramid"), int(QAbstract3DSeries::MeshPyramid));
    barStyleList->addItem(QStringLiteral("Cone"), int(QAbstract3DSeries::MeshCone));
    barStyleList->addItem(QStringLiteral("Cylinder"), int(QAbstract3DSeries::MeshCylinder));
    barStyleList->addItem(QStringLiteral("Bevel bar"), int(QAbstract3DSeries::MeshBevelBar));
    barStyleList->addItem(QStringLiteral("Sphere"), int(QAbstract3DSeries::MeshSphere));
    barStyleList->setCurrentIndex(4);

    QPushButton *cameraButton = new QPushButton(widget);
    cameraButton->setText(QStringLiteral("Change camera preset"));

    QPushButton *zoomToSelectedButton = new QPushButton(widget);
    zoomToSelectedButton->setText(QStringLiteral("Zoom to selected bar"));

    QComboBox *selectionModeList = new QComboBox(widget);
    selectionModeList->addItem(QStringLiteral("None"),
                               int(QAbstract3DGraph::SelectionNone));
    selectionModeList->addItem(QStringLiteral("Bar"),
                               int(QAbstract3DGraph::SelectionItem));
    selectionModeList->addItem(QStringLiteral("Row"),
                               int(QAbstract3DGraph::SelectionRow));
    selectionModeList->addItem(QStringLiteral("Bar and Row"),
                               int(QAbstract3DGraph::SelectionItemAndRow));
    selectionModeList->addItem(QStringLiteral("Column"),
                               int(QAbstract3DGraph::SelectionColumn));
    selectionModeList->addItem(QStringLiteral("Bar and Column"),
                               int(QAbstract3DGraph::SelectionItemAndColumn));
    selectionModeList->addItem(QStringLiteral("Row and Column"),
                               int(QAbstract3DGraph::SelectionRowAndColumn));
    selectionModeList->addItem(QStringLiteral("Bar, Row and Column"),
                               int(QAbstract3DGraph::SelectionItemRowAndColumn));
    selectionModeList->addItem(QStringLiteral("Slice into Row"),
                               int(QAbstract3DGraph::SelectionSlice | QAbstract3DGraph::SelectionRow));
    selectionModeList->addItem(QStringLiteral("Slice into Row and Item"),
                               int(QAbstract3DGraph::SelectionSlice | QAbstract3DGraph::SelectionItemAndRow));
    selectionModeList->addItem(QStringLiteral("Slice into Column"),
                               int(QAbstract3DGraph::SelectionSlice | QAbstract3DGraph::SelectionColumn));
    selectionModeList->addItem(QStringLiteral("Slice into Column and Item"),
                               int(QAbstract3DGraph::SelectionSlice | QAbstract3DGraph::SelectionItemAndColumn));
    selectionModeList->addItem(QStringLiteral("Multi: Bar, Row, Col"),
                               int(QAbstract3DGraph::SelectionItemRowAndColumn
                                   | QAbstract3DGraph::SelectionMultiSeries));
    selectionModeList->addItem(QStringLiteral("Multi, Slice: Row, Item"),
                               int(QAbstract3DGraph::SelectionSlice | QAbstract3DGraph::SelectionItemAndRow
                                   | QAbstract3DGraph::SelectionMultiSeries));
    selectionModeList->addItem(QStringLiteral("Multi, Slice: Col, Item"),
                               int(QAbstract3DGraph::SelectionSlice | QAbstract3DGraph::SelectionItemAndColumn
                                   | QAbstract3DGraph::SelectionMultiSeries));
    selectionModeList->setCurrentIndex(1);


    QCheckBox *gridCheckBox = new QCheckBox(widget);
    gridCheckBox->setText(QStringLiteral("Show grid"));
    gridCheckBox->setChecked(true);

    QCheckBox *seriesCheckBox = new QCheckBox(widget);
    seriesCheckBox->setText(QStringLiteral("Show second series"));
    seriesCheckBox->setChecked(false);



    QSlider *rotationSliderX = new QSlider(Qt::Horizontal, widget);
    rotationSliderX->setTickInterval(30);
    rotationSliderX->setTickPosition(QSlider::TicksBelow);
    rotationSliderX->setMinimum(-180);
    rotationSliderX->setValue(0);
    rotationSliderX->setMaximum(180);
    QSlider *rotationSliderY = new QSlider(Qt::Horizontal, widget);
    rotationSliderY->setTickInterval(15);
    rotationSliderY->setTickPosition(QSlider::TicksAbove);
    rotationSliderY->setMinimum(-90);
    rotationSliderY->setValue(0);
    rotationSliderY->setMaximum(90);


    QSlider *fontSizeSlider = new QSlider(Qt::Horizontal, widget);
    fontSizeSlider->setTickInterval(10);
    fontSizeSlider->setTickPosition(QSlider::TicksBelow);
    fontSizeSlider->setMinimum(1);
    fontSizeSlider->setValue(30);
    fontSizeSlider->setMaximum(100);

    QFontComboBox *fontList = new QFontComboBox(widget);
    fontList->setCurrentFont(QFont("Times New Roman"));

    QComboBox *rangeList = new QComboBox(widget);

    for(int i=1;i<=15;i++)
    {
        QString s=QString::number(i);
        rangeList->addItem(s);
    }
    rangeList->addItem(QStringLiteral("All"));
    rangeList->setCurrentIndex(1);

    QCheckBox *axisTitlesVisibleCB = new QCheckBox(widget);
    axisTitlesVisibleCB->setText(QStringLiteral("Axis titles visible"));
    axisTitlesVisibleCB->setChecked(true);

    QSlider *axisLabelRotationSlider = new QSlider(Qt::Horizontal, widget);
    axisLabelRotationSlider->setTickInterval(10);
    axisLabelRotationSlider->setTickPosition(QSlider::TicksBelow);
    axisLabelRotationSlider->setMinimum(0);
    axisLabelRotationSlider->setValue(30);
    axisLabelRotationSlider->setMaximum(90);


    vLayout->addWidget(new QLabel(QStringLiteral("Rotate horizontally")));
    vLayout->addWidget(rotationSliderX, 0, Qt::AlignTop);
    vLayout->addWidget(new QLabel(QStringLiteral("Rotate vertically")));
    vLayout->addWidget(rotationSliderY, 0, Qt::AlignTop);


    vLayout->addWidget(cameraButton, 0, Qt::AlignTop);
    vLayout->addWidget(zoomToSelectedButton, 0, Qt::AlignTop);
    vLayout->addWidget(gridCheckBox);
    vLayout->addWidget(seriesCheckBox);
    vLayout->addWidget(axisTitlesVisibleCB);
    vLayout->addWidget(new QLabel(QStringLiteral("Select Grid Number")));
    vLayout->addWidget(rangeList);
    vLayout->addWidget(new QLabel(QStringLiteral("Change bar style")));
    vLayout->addWidget(barStyleList);
    vLayout->addWidget(new QLabel(QStringLiteral("Change selection mode")));
    vLayout->addWidget(selectionModeList);
    vLayout->addWidget(new QLabel(QStringLiteral("Change font")));
    vLayout->addWidget(fontList);
    vLayout->addWidget(new QLabel(QStringLiteral("Adjust font size")));
    vLayout->addWidget(fontSizeSlider);
    vLayout->addWidget(new QLabel(QStringLiteral("Axis label rotation")));
    vLayout->addWidget(axisLabelRotationSlider, 1, Qt::AlignTop);


    GraphModifier *modifier = new GraphModifier(widgetgraph,gridStartPerDay,gridEndPerDay,gridFeesPerDay);


    QObject::connect(rotationSliderX, &QSlider::valueChanged, modifier, &GraphModifier::rotateX);
    QObject::connect(rotationSliderY, &QSlider::valueChanged, modifier, &GraphModifier::rotateY);


    QObject::connect(cameraButton, &QPushButton::clicked, modifier,
                     &GraphModifier::changePresetCamera);
    QObject::connect(zoomToSelectedButton, &QPushButton::clicked, modifier,
                     &GraphModifier::zoomToSelectedBar);
    QObject::connect(gridCheckBox, &QCheckBox::stateChanged, modifier,
                     &GraphModifier::setGridEnabled);
    QObject::connect(seriesCheckBox, &QCheckBox::stateChanged, modifier,
                     &GraphModifier::setSeriesVisibility);
    QObject::connect(modifier, &GraphModifier::gridEnabledChanged,
                     gridCheckBox, &QCheckBox::setChecked);

    QObject::connect(rangeList, SIGNAL(currentIndexChanged(int)), modifier,
                     SLOT(changeRange(int)));

    QObject::connect(barStyleList, SIGNAL(currentIndexChanged(int)), modifier,
                     SLOT(changeStyle(int)));

    QObject::connect(selectionModeList, SIGNAL(currentIndexChanged(int)), modifier,
                     SLOT(changeSelectionMode(int)));
    QObject::connect(widgetgraph, &Q3DBars::shadowQualityChanged, modifier,
                     &GraphModifier::shadowQualityUpdatedByVisual);

    QObject::connect(fontSizeSlider, &QSlider::valueChanged, modifier,
                     &GraphModifier::changeFontSize);
    QObject::connect(fontList, &QFontComboBox::currentFontChanged, modifier,
                     &GraphModifier::changeFont);

    QObject::connect(modifier, &GraphModifier::fontSizeChanged, fontSizeSlider,
                     &QSlider::setValue);
    QObject::connect(modifier, &GraphModifier::fontChanged, fontList,
                     &QFontComboBox::setCurrentFont);

    QObject::connect(axisTitlesVisibleCB, &QCheckBox::stateChanged, modifier,
                     &GraphModifier::setAxisTitleVisibility);
    QObject::connect(axisLabelRotationSlider, &QSlider::valueChanged, modifier,
                     &GraphModifier::changeLabelRotation);
    widget->show();
}
void MainWindow::releaseButton()
{
    ui->draw3DGraph->setEnabled(true);
    ui->chartButton->setEnabled(true);
}
