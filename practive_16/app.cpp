#include "app.h"
#include "ui_app.h"

App::App(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::App)
{
    ui->setupUi(this);

    customPlot = ui->widget;

    customPlot->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom);
    QCPGraph *graph = customPlot->addGraph();
    int n = 500;
    double phase = 0;
    double k = 3;
    QVector<double> x(n), y(n);
    for (int i=0; i<n; ++i)
    {
      x[i] = i/(double)(n-1)*34 - 17;
      y[i] = qExp(-x[i]*x[i]/20.0)*qSin(k*x[i]+phase);
    }
    graph->setData(x, y);
    graph->setPen(QPen(Qt::blue));
    graph->rescaleKeyAxis();
    customPlot->yAxis->setRange(-1.45, 1.65);
    customPlot->xAxis->grid()->setZeroLinePen(Qt::NoPen);

    // add the bracket at the top:
    // 在顶部添加括号:
    QCPItemBracket *bracket = new QCPItemBracket(customPlot);
    bracket->left->setCoords(-8, 1.1);
    bracket->right->setCoords(8, 1.1);
    bracket->setLength(13);

    // add the text label at the top:
    // 在顶部添加文本标签:
    QCPItemText *wavePacketText = new QCPItemText(customPlot);
    wavePacketText->position->setParentAnchor(bracket->center);
    wavePacketText->position->setCoords(0, -10); // move 10 pixels to the top from bracket center anchor
    wavePacketText->setPositionAlignment(Qt::AlignBottom|Qt::AlignHCenter);
    wavePacketText->setText("Wavepacket");
    wavePacketText->setFont(QFont(font().family(), 10));

    // add the phase tracer (red circle) which sticks to the graph data (and gets updated in bracketDataSlot by timer event):
    // 添加相位跟踪器(红圈)，它固定在图形数据上(并通过计时器事件在bracketDataSlot中更新):
    QCPItemTracer *phaseTracer = new QCPItemTracer(customPlot);
    itemDemoPhaseTracer = phaseTracer; // so we can access it later in the bracketDataSlot for animation    // 所以我们可以稍后在bracketDataSlot中访问它用于动画
    phaseTracer->setGraph(graph);
    phaseTracer->setGraphKey((M_PI*1.5-phase)/k);
    phaseTracer->setInterpolating(true);
    phaseTracer->setStyle(QCPItemTracer::tsCircle);
    phaseTracer->setPen(QPen(Qt::red));
    phaseTracer->setBrush(Qt::red);
    phaseTracer->setSize(7);

    // add label for phase tracer:
    // 为相位示踪器添加标签:
    QCPItemText *phaseTracerText = new QCPItemText(customPlot);
    phaseTracerText->position->setType(QCPItemPosition::ptAxisRectRatio);
    phaseTracerText->setPositionAlignment(Qt::AlignRight|Qt::AlignBottom);
    phaseTracerText->position->setCoords(1.0, 0.95); // lower right corner of axis rect
    phaseTracerText->setText("Points of fixed\nphase define\nphase velocity vp");
    phaseTracerText->setTextAlignment(Qt::AlignLeft);
    phaseTracerText->setFont(QFont(font().family(), 9));
    phaseTracerText->setPadding(QMargins(8, 0, 0, 0));

    // add arrow pointing at phase tracer, coming from label:
    // 添加箭头指向相位示踪器，来自标签:
    QCPItemCurve *phaseTracerArrow = new QCPItemCurve(customPlot);
    phaseTracerArrow->start->setParentAnchor(phaseTracerText->left);
    phaseTracerArrow->startDir->setParentAnchor(phaseTracerArrow->start);
    phaseTracerArrow->startDir->setCoords(-40, 0); // direction 30 pixels to the left of parent anchor (tracerArrow->start)
    phaseTracerArrow->end->setParentAnchor(phaseTracer->position);
    phaseTracerArrow->end->setCoords(10, 10);
    phaseTracerArrow->endDir->setParentAnchor(phaseTracerArrow->end);
    phaseTracerArrow->endDir->setCoords(30, 30);
    phaseTracerArrow->setHead(QCPLineEnding::esSpikeArrow);
    phaseTracerArrow->setTail(QCPLineEnding(QCPLineEnding::esBar, (phaseTracerText->bottom->pixelPosition().y()-phaseTracerText->top->pixelPosition().y())*0.85));

    // add the group velocity tracer (green circle):
    // 添加群速度示踪器(绿色圆圈):
    QCPItemTracer *groupTracer = new QCPItemTracer(customPlot);
    groupTracer->setGraph(graph);
    groupTracer->setGraphKey(5.5);
    groupTracer->setInterpolating(true);
    groupTracer->setStyle(QCPItemTracer::tsCircle);
    groupTracer->setPen(QPen(Qt::green));
    groupTracer->setBrush(Qt::green);
    groupTracer->setSize(7);

    // add label for group tracer:
    // 为组跟踪器添加标签:
    QCPItemText *groupTracerText = new QCPItemText(customPlot);
    groupTracerText->position->setType(QCPItemPosition::ptAxisRectRatio);
    groupTracerText->setPositionAlignment(Qt::AlignRight|Qt::AlignTop);
    groupTracerText->position->setCoords(1.0, 0.20); // lower right corner of axis rect
    groupTracerText->setText("Fixed positions in\nwave packet define\ngroup velocity vg");
    groupTracerText->setTextAlignment(Qt::AlignLeft);
    groupTracerText->setFont(QFont(font().family(), 9));
    groupTracerText->setPadding(QMargins(8, 0, 0, 0));

    // add arrow pointing at group tracer, coming from label:
    // 添加箭头指向组跟踪器，来自标签:
    QCPItemCurve *groupTracerArrow = new QCPItemCurve(customPlot);
    groupTracerArrow->start->setParentAnchor(groupTracerText->left);
    groupTracerArrow->startDir->setParentAnchor(groupTracerArrow->start);
    groupTracerArrow->startDir->setCoords(-40, 0); // direction 30 pixels to the left of parent anchor (tracerArrow->start)
    groupTracerArrow->end->setCoords(5.5, 0.4);
    groupTracerArrow->endDir->setParentAnchor(groupTracerArrow->end);
    groupTracerArrow->endDir->setCoords(0, -40);
    groupTracerArrow->setHead(QCPLineEnding::esSpikeArrow);
    groupTracerArrow->setTail(QCPLineEnding(QCPLineEnding::esBar, (groupTracerText->bottom->pixelPosition().y()-groupTracerText->top->pixelPosition().y())*0.85));

    // add dispersion arrow:
    // 添加分散箭:
    QCPItemCurve *arrow = new QCPItemCurve(customPlot);
    arrow->start->setCoords(1, -1.1);
    arrow->startDir->setCoords(-1, -1.3);
    arrow->endDir->setCoords(-5, -0.3);
    arrow->end->setCoords(-10, -0.2);
    arrow->setHead(QCPLineEnding::esSpikeArrow);

    // add the dispersion arrow label:
    // 添加色散箭头标签:
    QCPItemText *dispersionText = new QCPItemText(customPlot);
    dispersionText->position->setCoords(-6, -0.9);
    dispersionText->setRotation(40);
    dispersionText->setText("Dispersion with\nvp < vg");
    dispersionText->setFont(QFont(font().family(), 10));

    dataTimer = new QTimer(this);
    // setup a timer that repeatedly calls MainWindow::bracketDataSlot:
    connect(dataTimer, &QTimer::timeout, this, &App::bracketDataSlot);
    // connect(&dataTimer, SIGNAL(timeout()), this, SLOT(bracketDataSlot()));
    dataTimer->start(0); // Interval 0 means to refresh as fast as possible
}

App::~App()
{
    delete ui;
}

void App::bracketDataSlot()
{
    double secs = QCPAxisTickerDateTime::dateTimeToKey(QDateTime::currentDateTime());

    // update data to make phase move:
    // 更新数据使相移:
    int n = 500;
    double phase = secs*5;
    double k = 3;
    QVector<double> x(n), y(n);
    for (int i=0; i<n; ++i)
    {
      x[i] = i/(double)(n-1)*34 - 17;
      y[i] = qExp(-x[i]*x[i]/20.0)*qSin(k*x[i]+phase);
    }
    ui->widget->graph()->setData(x, y);

    itemDemoPhaseTracer->setGraphKey((8*M_PI+fmod(M_PI*1.5-phase, 6*M_PI))/k);

    ui->widget->replot();

    // calculate frames per second:
    // 计算每秒帧数:
    double key = secs;
    static double lastFpsKey;
    static int frameCount;
    ++frameCount;
    if (key-lastFpsKey > 2) // average fps over 2 seconds
    {
      ui->statusbar->showMessage(
            QString("%1 FPS, Total Data points: %2")
            .arg(frameCount/(key-lastFpsKey), 0, 'f', 0)
            .arg(ui->widget->graph(0)->data()->size())
            , 0);
      lastFpsKey = key;
      frameCount = 0;
    }
}
