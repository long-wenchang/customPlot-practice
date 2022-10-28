#include "app.h"
#include "ui_app.h"

App::App(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::App)
{
    ui->setupUi(this);

    customPlot = ui->widget;

    customPlot->setNoAntialiasingOnDrag(true); // more performance/responsiveness during dragging   // 拖拽过程中更高的性能/响应性
    customPlot->addGraph();
    QPen pen;
    pen.setColor(QColor(255,170,100));
    pen.setWidth(2);
    pen.setStyle(Qt::DotLine);
    customPlot->graph(0)->setPen(pen);
    customPlot->graph(0)->setName("x");

    customPlot->addGraph();
    customPlot->graph(1)->setPen(QPen(Qt::red));
    customPlot->graph(1)->setBrush(QBrush(QColor(255, 0, 0, 20)));
    customPlot->graph(1)->setName("-sin(x)exp(x)");

    customPlot->addGraph();
    customPlot->graph(2)->setPen(QPen(Qt::blue));
    customPlot->graph(2)->setBrush(QBrush(QColor(0, 0, 255, 20)));
    customPlot->graph(2)->setName(" sin(x)exp(x)");

    customPlot->addGraph();
    pen.setColor(QColor(0,0,0));
    pen.setWidth(1);
    pen.setStyle(Qt::DashLine);
    customPlot->graph(3)->setPen(pen);
    customPlot->graph(3)->setBrush(QBrush(QColor(0,0,0,15)));
    customPlot->graph(3)->setLineStyle(QCPGraph::lsStepCenter);
    customPlot->graph(3)->setName("x!");

    const int dataCount = 200;
    const int dataFactorialCount = 21;
    QVector<QCPGraphData> dataLinear(dataCount), dataMinusSinExp(dataCount), dataPlusSinExp(dataCount), dataFactorial(dataFactorialCount);
    for (int i=0; i<dataCount; ++i)
    {
      dataLinear[i].key = i/10.0;
      dataLinear[i].value = dataLinear[i].key;
      dataMinusSinExp[i].key = i/10.0;
      dataMinusSinExp[i].value = -qSin(dataMinusSinExp[i].key)*qExp(dataMinusSinExp[i].key);
      dataPlusSinExp[i].key = i/10.0;
      dataPlusSinExp[i].value = qSin(dataPlusSinExp[i].key)*qExp(dataPlusSinExp[i].key);
    }
    for (int i=0; i<dataFactorialCount; ++i)
    {
      dataFactorial[i].key = i;
      dataFactorial[i].value = 1.0;
      for (int k=1; k<=i; ++k) dataFactorial[i].value *= k; // factorial
    }
    customPlot->graph(0)->data()->set(dataLinear);
    customPlot->graph(1)->data()->set(dataMinusSinExp);
    customPlot->graph(2)->data()->set(dataPlusSinExp);
    customPlot->graph(3)->data()->set(dataFactorial);

    customPlot->yAxis->grid()->setSubGridVisible(true);
    customPlot->xAxis->grid()->setSubGridVisible(true);
    customPlot->yAxis->setScaleType(QCPAxis::stLogarithmic);
    customPlot->yAxis2->setScaleType(QCPAxis::stLogarithmic);
    QSharedPointer<QCPAxisTickerLog> logTicker(new QCPAxisTickerLog);
    customPlot->yAxis->setTicker(logTicker);
    customPlot->yAxis2->setTicker(logTicker);
    customPlot->yAxis->setNumberFormat("eb"); // e = exponential, b = beautiful decimal powers      // E =指数，b =漂亮的小数次方
    customPlot->yAxis->setNumberPrecision(0); // makes sure "1*10^4" is displayed only as "10^4"    // 确保“1*10^4”只显示为“10^4”
    customPlot->xAxis->setRange(0, 19.9);
    customPlot->yAxis->setRange(1e-2, 1e10);
    // make range draggable and zoomable:
    // 使范围可拖动和可缩放:
    customPlot->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom);

    // make top right axes clones of bottom left axes:
    // 将右上轴复制为左下轴:
    customPlot->axisRect()->setupFullAxesBox();
    // connect signals so top and right axes move in sync with bottom and left axes:
    // 连接信号，使上下轴与上下轴同步移动:
    connect(customPlot->xAxis, SIGNAL(rangeChanged(QCPRange)), customPlot->xAxis2, SLOT(setRange(QCPRange)));
    connect(customPlot->yAxis, SIGNAL(rangeChanged(QCPRange)), customPlot->yAxis2, SLOT(setRange(QCPRange)));

    customPlot->legend->setVisible(true);
    customPlot->legend->setBrush(QBrush(QColor(255,255,255,150)));
    customPlot->axisRect()->insetLayout()->setInsetAlignment(0, Qt::AlignLeft|Qt::AlignTop); // make legend align in top left corner or axis rect
}

App::~App()
{
    delete ui;
}

