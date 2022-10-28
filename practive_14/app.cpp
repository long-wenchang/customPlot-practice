#include "app.h"
#include "ui_app.h"

App::App(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::App)
{
    ui->setupUi(this);

    customPlot = ui->widget;

    QCPStatisticalBox *statistical = new QCPStatisticalBox(customPlot->xAxis, customPlot->yAxis);
    QBrush boxBrush(QColor(60, 60, 255, 100));
    boxBrush.setStyle(Qt::Dense6Pattern); // make it look oldschool
    statistical->setBrush(boxBrush);

    // specify data:
    statistical->addData(1, 1.1, 1.9, 2.25, 2.7, 4.2);
    statistical->addData(2, 0.8, 1.6, 2.2, 3.2, 4.9, QVector<double>() << 0.7 << 0.34 << 0.45 << 6.2 << 5.84); // provide some outliers as QVector
    statistical->addData(3, 0.2, 0.7, 1.1, 1.6, 2.9);

    // prepare manual x axis labels:
    customPlot->xAxis->setSubTicks(false);
    customPlot->xAxis->setTickLength(0, 4);
    customPlot->xAxis->setTickLabelRotation(20);
    QSharedPointer<QCPAxisTickerText> textTicker(new QCPAxisTickerText);
    textTicker->addTick(1, "Sample 1");
    textTicker->addTick(2, "Sample 2");
    textTicker->addTick(3, "Control Group");
    customPlot->xAxis->setTicker(textTicker);

    // prepare axes:
    customPlot->yAxis->setLabel(QString::fromUtf8("O₂ Absorption [mg]"));
    customPlot->rescaleAxes();
    customPlot->xAxis->scaleRange(1.7, customPlot->xAxis->range().center());
    customPlot->yAxis->setRange(0, 7);
    customPlot->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom);
}

App::~App()
{
    delete ui;
}

