#include "graphmodifier.h"
#include <QtDataVisualization/qcategory3daxis.h>
#include <QtDataVisualization/qvalue3daxis.h>
#include <QtDataVisualization/qbardataproxy.h>
#include <QtDataVisualization/q3dscene.h>
#include <QtDataVisualization/q3dcamera.h>
#include <QtDataVisualization/qbar3dseries.h>
#include <QtDataVisualization/q3dtheme.h>
#include <QtCore/QTime>
#include <QtWidgets/QComboBox>
#include <QtCore/qmath.h>

using namespace QtDataVisualization;
GraphModifier::GraphModifier(Q3DBars *bargraph,QVector<QVector<int>> gridStartPerDay,QVector<QVector<int>> gridEndPerDay,    QVector<QVector<double>> gridFeesPerDay)
    : m_graph(bargraph),
      m_xRotation(0.0f),
      m_yRotation(0.0f),
      m_fontSize(10),
      m_segments(4),
      m_subSegments(3),
      m_minval(0.0f),
      m_maxval(20000.0f),

      m_ordersAxis(new QValue3DAxis),
      m_dateAxis(new QCategory3DAxis),
      m_gridAxis(new QCategory3DAxis),
      m_primarySeries(new QBar3DSeries),
      m_secondarySeries(new QBar3DSeries),

      m_barMesh(QAbstract3DSeries::MeshBevelBar),
      m_smooth(false)
{

    m_graph->setShadowQuality(QAbstract3DGraph::ShadowQualitySoftMedium);
    m_graph->activeTheme()->setBackgroundEnabled(false);
    m_graph->activeTheme()->setFont(QFont("Times New Roman", m_fontSize));
    m_graph->activeTheme()->setLabelBackgroundEnabled(true);
    m_graph->setMultiSeriesUniform(true);


    for(int i=1;i<=15;i++)
        m_dates<<"2016 11 "+QString::number(i);
    for(int i=0;i<=99;i++)
        m_grids << QString::number(i);

    m_ordersAxis->setTitle("Didi Taxi Data");
    m_ordersAxis->setSegmentCount(m_segments);
    m_ordersAxis->setSubSegmentCount(m_subSegments);
    m_ordersAxis->setRange(m_minval, m_maxval);
    m_ordersAxis->setLabelFormat(QString(" "+QStringLiteral("%.0f ")));
    m_ordersAxis->setLabelAutoRotation(30.0f);
    m_ordersAxis->setTitleVisible(true);

    m_dateAxis->setTitle("Time");
    m_dateAxis->setLabelAutoRotation(30.0f);
    m_dateAxis->setTitleVisible(true);
    m_gridAxis->setTitle("Grid");
    m_gridAxis->setLabelAutoRotation(30.0f);
    m_gridAxis->setTitleVisible(true);

    m_graph->setValueAxis(m_ordersAxis);
    m_graph->setRowAxis(m_dateAxis);
    m_graph->setColumnAxis(m_gridAxis);


    m_primarySeries->setItemLabelFormat(QStringLiteral("Grid:@colLabel Date:@rowLabel Departures:@valueLabel"));
    m_primarySeries->setMesh(QAbstract3DSeries::MeshBevelBar);
    m_primarySeries->setMeshSmooth(false);

    m_secondarySeries->setItemLabelFormat(QStringLiteral("Grid:@colLabel Date:@rowLabel Total Fees:@valueLabel"));
    m_secondarySeries->setMesh(QAbstract3DSeries::MeshBevelBar);
    m_secondarySeries->setMeshSmooth(false);
    m_secondarySeries->setVisible(false);


    m_graph->addSeries(m_primarySeries);
    m_graph->addSeries(m_secondarySeries);


    changePresetCamera();


    resetData(gridStartPerDay,gridEndPerDay,gridFeesPerDay);


    Q3DCamera *camera = m_graph->scene()->activeCamera();
    m_defaultAngleX = camera->xRotation();
    m_defaultAngleY = camera->yRotation();
    m_defaultZoom = camera->zoomLevel();
    m_defaultTarget = camera->target();

    m_animationCameraX.setTargetObject(camera);
    m_animationCameraY.setTargetObject(camera);
    m_animationCameraZoom.setTargetObject(camera);
    m_animationCameraTarget.setTargetObject(camera);

    m_animationCameraX.setPropertyName("xRotation");
    m_animationCameraY.setPropertyName("yRotation");
    m_animationCameraZoom.setPropertyName("zoomLevel");
    m_animationCameraTarget.setPropertyName("target");

    int duration = 1700;
    m_animationCameraX.setDuration(duration);
    m_animationCameraY.setDuration(duration);
    m_animationCameraZoom.setDuration(duration);
    m_animationCameraTarget.setDuration(duration);


    qreal zoomOutFraction = 0.3;
    m_animationCameraX.setKeyValueAt(zoomOutFraction, QVariant::fromValue(0.0f));
    m_animationCameraY.setKeyValueAt(zoomOutFraction, QVariant::fromValue(90.0f));
    m_animationCameraZoom.setKeyValueAt(zoomOutFraction, QVariant::fromValue(50.0f));
    m_animationCameraTarget.setKeyValueAt(zoomOutFraction,
    QVariant::fromValue(QVector3D(0.0f, 0.0f, 0.0f)));

}


GraphModifier::~GraphModifier()
{
    delete m_graph;
}

void GraphModifier::resetData(QVector<QVector<int>> gridStartPerDay,QVector<QVector<int>> gridEndPerDay,    QVector<QVector<double>> gridFeesPerDay)
{


    static float ** tempOulu ;
    tempOulu=new float* [gridStartPerDay[0].size()];
    for(int i=0;i<gridStartPerDay[0].size();i++)
        tempOulu[i]=new float [gridStartPerDay.size()];
    for(int i=0;i<gridStartPerDay.size();i++)
        for(int j=0;j<gridStartPerDay[0].size();j++)
        {
            tempOulu[j][i]=gridStartPerDay[i][j];
        }

    static float ** tempHelsinki ;
    tempHelsinki=new float* [gridFeesPerDay[0].size()];
    for(int i=0;i<gridFeesPerDay[0].size();i++)
        tempHelsinki[i]=new float [gridFeesPerDay.size()];
    for(int i=0;i<gridFeesPerDay.size();i++)
        for(int j=0;j<gridFeesPerDay[0].size();j++)
        {
            tempHelsinki[j][i]=gridEndPerDay[i][j];
        }



    QBarDataArray *dataSet = new QBarDataArray;
    QBarDataArray *dataSet2 = new QBarDataArray;
    QBarDataRow *dataRow;
    QBarDataRow *dataRow2;

    dataSet->reserve(m_dates.size());
    for (int year = 0; year < m_dates.size(); year++) {
        dataRow = new QBarDataRow(m_grids.size());
        dataRow2 = new QBarDataRow(m_grids.size());
        for (int month = 0; month < m_grids.size(); month++) {
            (*dataRow)[month].setValue(tempOulu[year][month]);
            (*dataRow2)[month].setValue(tempHelsinki[year][month]);
        }
        dataSet->append(dataRow);
        dataSet2->append(dataRow2);
    }

    m_primarySeries->dataProxy()->resetArray(dataSet, m_dates, m_grids);
    m_secondarySeries->dataProxy()->resetArray(dataSet2, m_dates, m_grids);
}

void GraphModifier::changeRange(int range)
{
    if (range >= m_dates.count())
        m_dateAxis->setRange(0, m_dates.count() - 1);
    else
        m_dateAxis->setRange(range+1, range+1);
}

void GraphModifier::changeStyle(int style)
{
    QComboBox *comboBox = qobject_cast<QComboBox *>(sender());
    if (comboBox) {
        m_barMesh = QAbstract3DSeries::Mesh(comboBox->itemData(style).toInt());
        m_primarySeries->setMesh(m_barMesh);
        m_secondarySeries->setMesh(m_barMesh);
    }
}

void GraphModifier::changePresetCamera()
{
    m_animationCameraX.stop();
    m_animationCameraY.stop();
    m_animationCameraZoom.stop();
    m_animationCameraTarget.stop();

    m_graph->scene()->activeCamera()->setTarget(QVector3D(0.0f, 0.0f, 0.0f));


    static int preset = Q3DCamera::CameraPresetFront;

    m_graph->scene()->activeCamera()->setCameraPreset((Q3DCamera::CameraPreset)preset);

    if (++preset > Q3DCamera::CameraPresetDirectlyBelow)
        preset = Q3DCamera::CameraPresetFrontLow;

}

void GraphModifier::changeTheme(int theme)
{
    Q3DTheme *currentTheme = m_graph->activeTheme();
    currentTheme->setType(Q3DTheme::Theme(theme));
    emit backgroundEnabledChanged(currentTheme->isBackgroundEnabled());
    emit gridEnabledChanged(currentTheme->isGridEnabled());
    emit fontChanged(currentTheme->font());
    emit fontSizeChanged(currentTheme->font().pointSize());
}

void GraphModifier::changeLabelBackground()
{
    m_graph->activeTheme()->setLabelBackgroundEnabled(!m_graph->activeTheme()->isLabelBackgroundEnabled());
}

void GraphModifier::changeSelectionMode(int selectionMode)
{
    QComboBox *comboBox = qobject_cast<QComboBox *>(sender());
    if (comboBox) {
        int flags = comboBox->itemData(selectionMode).toInt();
        m_graph->setSelectionMode(QAbstract3DGraph::SelectionFlags(flags));
    }
}

void GraphModifier::changeFont(const QFont &font)
{
    QFont newFont = font;
    m_graph->activeTheme()->setFont(newFont);
}

void GraphModifier::changeFontSize(int fontsize)
{
    m_fontSize = fontsize;
    QFont font = m_graph->activeTheme()->font();
    font.setPointSize(m_fontSize);
    m_graph->activeTheme()->setFont(font);
}

void GraphModifier::shadowQualityUpdatedByVisual(QAbstract3DGraph::ShadowQuality sq)
{
    int quality = int(sq);

    emit shadowQualityChanged(quality);
}

void GraphModifier::changeLabelRotation(int rotation)
{
    m_ordersAxis->setLabelAutoRotation(float(rotation));
    m_gridAxis->setLabelAutoRotation(float(rotation));
    m_dateAxis->setLabelAutoRotation(float(rotation));
}

void GraphModifier::setAxisTitleVisibility(bool enabled)
{
    m_ordersAxis->setTitleVisible(enabled);
    m_gridAxis->setTitleVisible(enabled);
    m_dateAxis->setTitleVisible(enabled);
}

void GraphModifier::setAxisTitleFixed(bool enabled)
{
    m_ordersAxis->setTitleFixed(enabled);
    m_gridAxis->setTitleFixed(enabled);
    m_dateAxis->setTitleFixed(enabled);
}

void GraphModifier::zoomToSelectedBar()
{
    m_animationCameraX.stop();
    m_animationCameraY.stop();
    m_animationCameraZoom.stop();
    m_animationCameraTarget.stop();

    Q3DCamera *camera = m_graph->scene()->activeCamera();
    float currentX = camera->xRotation();
    float currentY = camera->yRotation();
    float currentZoom = camera->zoomLevel();
    QVector3D currentTarget = camera->target();

    m_animationCameraX.setStartValue(QVariant::fromValue(currentX));
    m_animationCameraY.setStartValue(QVariant::fromValue(currentY));
    m_animationCameraZoom.setStartValue(QVariant::fromValue(currentZoom));
    m_animationCameraTarget.setStartValue(QVariant::fromValue(currentTarget));

    QPoint selectedBar = m_graph->selectedSeries()
            ? m_graph->selectedSeries()->selectedBar()
            : QBar3DSeries::invalidSelectionPosition();

    if (selectedBar != QBar3DSeries::invalidSelectionPosition()) {

        QVector3D endTarget;
        float xMin = m_graph->columnAxis()->min();
        float xRange = m_graph->columnAxis()->max() - xMin;
        float zMin = m_graph->rowAxis()->min();
        float zRange = m_graph->rowAxis()->max() - zMin;
        endTarget.setX((selectedBar.y() - xMin) / xRange * 2.0f - 1.0f);
        endTarget.setZ((selectedBar.x() - zMin) / zRange * 2.0f - 1.0f);


        qreal endAngleX = 90.0 - qRadiansToDegrees(qAtan(qreal(endTarget.z() / endTarget.x())));
        if (endTarget.x() > 0.0f)
            endAngleX -= 180.0f;
        float barValue = m_graph->selectedSeries()->dataProxy()->itemAt(selectedBar.x(),
                                                                        selectedBar.y())->value();
        float endAngleY = barValue >= 0.0f ? 30.0f : -30.0f;
        if (m_graph->valueAxis()->reversed())
            endAngleY *= -1.0f;


        m_animationCameraX.setEndValue(QVariant::fromValue(float(endAngleX)));
        m_animationCameraY.setEndValue(QVariant::fromValue(endAngleY));
        m_animationCameraZoom.setEndValue(QVariant::fromValue(250));

        m_animationCameraTarget.setEndValue(QVariant::fromValue(endTarget));

    } else {

        m_animationCameraX.setEndValue(QVariant::fromValue(m_defaultAngleX));
        m_animationCameraY.setEndValue(QVariant::fromValue(m_defaultAngleY));
        m_animationCameraZoom.setEndValue(QVariant::fromValue(m_defaultZoom));
        m_animationCameraTarget.setEndValue(QVariant::fromValue(m_defaultTarget));
    }

    m_animationCameraX.start();
    m_animationCameraY.start();
    m_animationCameraZoom.start();
    m_animationCameraTarget.start();
}


void GraphModifier::changeShadowQuality(int quality)
{
    QAbstract3DGraph::ShadowQuality sq = QAbstract3DGraph::ShadowQuality(quality);
    m_graph->setShadowQuality(sq);
    emit shadowQualityChanged(quality);
}


void GraphModifier::rotateX(int rotation)
{
    m_xRotation = rotation;
    m_graph->scene()->activeCamera()->setCameraPosition(m_xRotation, m_yRotation);
}

void GraphModifier::rotateY(int rotation)
{
    m_yRotation = rotation;
    m_graph->scene()->activeCamera()->setCameraPosition(m_xRotation, m_yRotation);
}


void GraphModifier::setBackgroundEnabled(int enabled)
{
    m_graph->activeTheme()->setBackgroundEnabled(bool(enabled));
}

void GraphModifier::setGridEnabled(int enabled)
{
    m_graph->activeTheme()->setGridEnabled(bool(enabled));
}

void GraphModifier::setSmoothBars(int smooth)
{
    m_smooth = bool(smooth);
    m_primarySeries->setMeshSmooth(m_smooth);
    m_secondarySeries->setMeshSmooth(m_smooth);
}

void GraphModifier::setSeriesVisibility(int enabled)
{
    m_secondarySeries->setVisible(bool(enabled));
}

void GraphModifier::setReverseValueAxis(int enabled)
{
    m_graph->valueAxis()->setReversed(enabled);
}

void GraphModifier::setReflection(bool enabled)
{
    m_graph->setReflection(enabled);
}
