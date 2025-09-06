#include "Widget.h"
#include "ui_widget.h"

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget) {
    ui->setupUi(this);
    timer = new QTimer(this);
    currentValue = 0;
    flag = false;
    connect(timer, &QTimer::timeout, [ = ]() {
        if (flag) {
            currentValue--;
            if (currentValue <= 0) {
                flag = false;
            }
        } else {
            currentValue++;
            if (currentValue >= 60) {
                flag = true;
            }
        }
        update();
    });
    timer->start();
    timer->setInterval(50);
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
    painter.save();//保存初始坐标系位置
    //画值
    painter.setFont(QFont("楷体", 30));
    painter.drawText(QRect(-60, -60, 120, 120), Qt::AlignCenter, QString::number(currentValue));
    //画大刻度
    //旋转坐标系
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
    painter.setFont(QFont("华文宋体", 15));
    double angle = 270.0 / 60.0;
    painter.rotate(180);
    for (int i = 0; i <= 60; i++) {
        painter.drawLine(height() / 2 - 6, 0, height() / 2 - 1, 0);
        if (i % 10 == 0) {
            if (i == 10 || i == 20) {
                painter.rotate(180);
                painter.drawText(-height() / 2 + 30, 4, QString::number(i));
                painter.rotate(-180);
            } else {
                painter.drawText(height() / 2 - 50, 4, QString::number(i));
            }
            //painter.drawText(height() / 2 - 50, 4, QString::number(i));注意第二个参数
        }
        painter.rotate(angle);
    }
    //画指针
    painter.restore();//坐标轴回到初始位置
    painter.save();
    painter.rotate(135 + currentValue * angle);
    painter.drawLine(height() / 8, 0, height() / 2 - 62, 0);
    //画扇形
    painter.restore();
    painter.setPen(Qt::NoPen);
    painter.setBrush(QColor(255, 0, 0, 150));
    painter.drawPie(QRect(-height() / 2 + 60, -height() / 2 + 60, height() - 120, height() - 120), (- 135) * 16, -angle * currentValue * 16);
}
