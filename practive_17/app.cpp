#include "app.h"
#include "ui_app.h"

App::App(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::App)
{
    ui->setupUi(this);

    customPlot = ui->customPlot;

    // configure axis rect:
    // 配置轴矩形:
    customPlot->plotLayout()->clear(); // clear default axis rect so we can start from scratch // 清除默认的坐标轴矩形，以便我们可以从头开始
    QCPAxisRect *wideAxisRect = new QCPAxisRect(customPlot);
    wideAxisRect->setupFullAxesBox(true);
    wideAxisRect->axis(QCPAxis::atRight, 0)->setTickLabels(true);
    wideAxisRect->addAxis(QCPAxis::atLeft)->setTickLabelColor(QColor("#6050F8")); // add an extra axis on the left and color its numbers    // 在左侧添加一个额外的轴，并为其数字着色
    QCPLayoutGrid *subLayout = new QCPLayoutGrid;
    customPlot->plotLayout()->addElement(0, 0, wideAxisRect); // insert axis rect in first row  // 在第一行插入轴矩形
    customPlot->plotLayout()->addElement(1, 0, subLayout); // sub layout in second row (grid layout will grow accordingly)  // 第二行的子布局(网格布局将相应地增长)
    //customPlot->plotLayout()->setRowStretchFactor(1, 2);
    // prepare axis rects that will be placed in the sublayout:
    // 准备坐标轴矩形，将放置在子布局:
    QCPAxisRect *subRectLeft = new QCPAxisRect(customPlot, false); // false means to not setup default axes // False表示不设置默认轴
    QCPAxisRect *subRectRight = new QCPAxisRect(customPlot, false);
    subLayout->addElement(0, 0, subRectLeft);
    subLayout->addElement(0, 1, subRectRight);
    subRectRight->setMaximumSize(150, 150); // make bottom right axis rect size fixed 150x150   // 使右下轴矩形尺寸固定为150 × 150
    subRectRight->setMinimumSize(150, 150); // make bottom right axis rect size fixed 150x150
    // setup axes in sub layout axis rects:
    // 设置轴在子布局轴矩形:
    subRectLeft->addAxes(QCPAxis::atBottom | QCPAxis::atLeft);
    subRectRight->addAxes(QCPAxis::atBottom | QCPAxis::atRight);
    subRectLeft->axis(QCPAxis::atLeft)->ticker()->setTickCount(2);
    subRectRight->axis(QCPAxis::atRight)->ticker()->setTickCount(2);
    subRectRight->axis(QCPAxis::atBottom)->ticker()->setTickCount(2);
    subRectLeft->axis(QCPAxis::atBottom)->grid()->setVisible(true);
    // synchronize the left and right margins of the top and bottom axis rects:
    // 同步上下轴矩形的左右边距:
    QCPMarginGroup *marginGroup = new QCPMarginGroup(customPlot);
    subRectLeft->setMarginGroup(QCP::msLeft, marginGroup);
    subRectRight->setMarginGroup(QCP::msRight, marginGroup);
    wideAxisRect->setMarginGroup(QCP::msLeft | QCP::msRight, marginGroup);
    // move newly created axes on "axes" layer and grids on "grid" layer:
    // 移动新创建的轴到“axes”层，网格在“grid”层:移动新创建的轴到“axes”层，网格在“grid”层:
    foreach (QCPAxisRect *rect, customPlot->axisRects())
    {
      foreach (QCPAxis *axis, rect->axes())
      {
        axis->setLayer("axes");
        axis->grid()->setLayer("grid");
      }
    }

    // prepare data:
    QVector<QCPGraphData> dataCos(21), dataGauss(50), dataRandom(100);
    QVector<double> x3, y3;
    qsrand(3);
    for (int i=0; i<dataCos.size(); ++i)
    {
      dataCos[i].key = i/(double)(dataCos.size()-1)*10-5.0;
      dataCos[i].value = qCos(dataCos[i].key);
    }
    for (int i=0; i<dataGauss.size(); ++i)
    {
      dataGauss[i].key = i/(double)dataGauss.size()*10-5.0;
      dataGauss[i].value = qExp(-dataGauss[i].key*dataGauss[i].key*0.2)*1000;
    }
    for (int i=0; i<dataRandom.size(); ++i)
    {
      dataRandom[i].key = i/(double)dataRandom.size()*10;
      dataRandom[i].value = qrand()/(double)RAND_MAX-0.5+dataRandom[qMax(0, i-1)].value;
    }
    x3 << 1 << 2 << 3 << 4;
    y3 << 2 << 2.5 << 4 << 1.5;

    // create and configure plottables:
    // 创建和配置绘图表:
    QCPGraph *mainGraphCos = customPlot->addGraph(wideAxisRect->axis(QCPAxis::atBottom), wideAxisRect->axis(QCPAxis::atLeft));
    mainGraphCos->data()->set(dataCos);
    mainGraphCos->valueAxis()->setRange(-1, 1);
    mainGraphCos->rescaleKeyAxis();
    mainGraphCos->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssCircle, QPen(Qt::black), QBrush(Qt::white), 6));
    mainGraphCos->setPen(QPen(QColor(120, 120, 120), 2));
    QCPGraph *mainGraphGauss = customPlot->addGraph(wideAxisRect->axis(QCPAxis::atBottom), wideAxisRect->axis(QCPAxis::atLeft, 1));
    mainGraphGauss->data()->set(dataGauss);
    mainGraphGauss->setPen(QPen(QColor("#8070B8"), 2));
    mainGraphGauss->setBrush(QColor(110, 170, 110, 30));
    mainGraphCos->setChannelFillGraph(mainGraphGauss);
    mainGraphCos->setBrush(QColor(255, 161, 0, 50));
    mainGraphGauss->valueAxis()->setRange(0, 1000);
    mainGraphGauss->rescaleKeyAxis();

    QCPGraph *subGraphRandom = customPlot->addGraph(subRectLeft->axis(QCPAxis::atBottom), subRectLeft->axis(QCPAxis::atLeft));
    subGraphRandom->data()->set(dataRandom);
    subGraphRandom->setLineStyle(QCPGraph::lsImpulse);
    subGraphRandom->setPen(QPen(QColor("#FFA100"), 1.5));
    subGraphRandom->rescaleAxes();

    QCPBars *subBars = new QCPBars(subRectRight->axis(QCPAxis::atBottom), subRectRight->axis(QCPAxis::atRight));
    subBars->setWidth(3/(double)x3.size());
    subBars->setData(x3, y3);
    subBars->setPen(QPen(Qt::black));
    subBars->setAntialiased(false);
    subBars->setAntialiasedFill(false);
    subBars->setBrush(QColor("#705BE8"));
    subBars->keyAxis()->setSubTicks(false);
    subBars->rescaleAxes();
    // setup a ticker for subBars key axis that only gives integer ticks:
    // 为subBars键轴设置一个只给出整数刻度的计时器:
    QSharedPointer<QCPAxisTickerFixed> intTicker(new QCPAxisTickerFixed);
    intTicker->setTickStep(1.0);
    intTicker->setScaleStrategy(QCPAxisTickerFixed::ssMultiples);
    subBars->keyAxis()->setTicker(intTicker);
}

App::~App()
{
    delete ui;
}

