#ifndef APP_H
#define APP_H

#include <QMainWindow>
#include <qcustomplot.h>

QT_BEGIN_NAMESPACE
namespace Ui { class App; }
QT_END_NAMESPACE

class App : public QMainWindow
{
    Q_OBJECT

public:
    App(QWidget *parent = nullptr);
    ~App();

private slots:
    void titleDoubleClick(QMouseEvent* event);

    void axisLabelDoubleClick(QCPAxis *axis, QCPAxis::SelectablePart part);

    void legendDoubleClick(QCPLegend *legend, QCPAbstractLegendItem *item);

    void selectionChanged();

    void mousePress();

    void mouseWheel();

    void addRandomGraph();

    void removeSelectedGraph();

    void removeAllGraphs();

    void contextMenuRequest(QPoint pos);

    void moveLegend();

    void graphClicked(QCPAbstractPlottable *plottable, int dataIndex);

private:
    Ui::App *ui;
};
#endif // APP_H
