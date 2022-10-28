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

private:
    Ui::App *ui;

    QCustomPlot * customPlot;
};
#endif // APP_H
