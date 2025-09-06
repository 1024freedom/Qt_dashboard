#include "Widget.h"
#include "ui_widget.h"

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget) {
    ui->setupUi(this);
}

Widget::~Widget() {
    delete ui;
}
void Widget::paintEvent(QPaintEvent *event) {
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing, true);
    painter.setBrush(Qt::black);
    painter.drawRect(rect());
    painter.translate(rect().center());

    QRadialGradient radialGradient(0, 0, height() / 2); //径向渐变
    radialGradient.setColorAt(0.0, QColor(255, 0, 0, 50)); //中心颜色
    radialGradient.setColorAt(1.0, QColor(255, 0, 0, 250)); //外围颜色

    QBrush brush(radialGradient);
    painter.setBrush(brush);
    //画大圆
    painter.drawEllipse(QPoint(0, 0), height() / 2, height() / 2);
    //画小圆
    painter.setPen(QPen(Qt::white, 3));
    painter.setBrush(Qt::NoBrush);
    painter.drawEllipse(QPoint(0, 0), height() / 8, height() / 8);
    //画大刻度
    painter.drawLine(height() / 2 - 15, 0, height() / 2 - 1, 0);
    painter.rotate(45);//旋转45度
    painter.drawLine(height() / 2 - 15, 0, height() / 2 - 1, 0);
    painter.rotate(90);//相比前一次的painter位置旋转90度
    painter.drawLine(height() / 2 - 15, 0, height() / 2 - 1, 0);
    for (int i = 0; i < 4; i++) {
        painter.rotate(45);
        painter.drawLine(height() / 2 - 15, 0, height() / 2 - 1, 0);
    }

    //画小刻度
    double angle = 270.0 / 60.0;
    painter.rotate(180);
    for (int i = 0; i < 60; i++) {
        painter.rotate(angle);
        painter.drawLine(height() / 2 - 6, 0, height() / 2 - 1, 0);
    }
}
