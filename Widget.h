#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QPainter>
#include <QPaintEvent>
#include<QTimer>
#include <QPointF>

QT_BEGIN_NAMESPACE
namespace Ui {
class Widget;
}
QT_END_NAMESPACE

class Widget : public QWidget {
    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);
    ~Widget();
protected:
    void paintEvent(QPaintEvent *event)override;
    void initCanvas(QPainter& painter);
    void drawMiddleCircle(QPainter& painter, int r);
    void drawMiddleValue(QPainter& painter);
    void drawGradation(QPainter& painter);
    void drawPointer(QPainter& painter);
    void draw_Pie(QPainter& painter, int r);
    void drawEllipseInnerBlack(QPainter& painter, int r);
    void drawEllipseInnerShine(QPainter& painter, int r);
private:
    Ui::Widget* ui;
    QTimer* timer;
    int currentValue;
    double angle;
    bool flag;//记录刻度状态
};
#endif // WIDGET_H
