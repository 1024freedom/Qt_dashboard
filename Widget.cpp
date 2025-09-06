#include "Widget.h"
#include "ui_widget.h"

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget) {
    ui->setupUi(this);
    timer = new QTimer(this);
    angle = 270.0 / 60.0;
    currentValue = 0;
    flag = false;
    connect(timer, &QTimer::timeout, this, [ = ]() {
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
void Widget::initCanvas(QPainter& painter) {
    painter.setRenderHint(QPainter::Antialiasing, true);
    painter.setBrush(Qt::black);
    painter.drawRect(rect());
    painter.translate(rect().center());

    // QRadialGradient radialGradient(0, 0, height() / 2); //径向渐变
    // radialGradient.setColorAt(0.0, QColor(255, 0, 0, 50)); //中心颜色
    // radialGradient.setColorAt(1.0, QColor(255, 0, 0, 250)); //外围颜色

    // QBrush brush(radialGradient);
    // painter.setBrush(brush);
    // //画大圆
    // painter.drawEllipse(QPoint(0, 0), height() / 2, height() / 2);
}
void Widget::drawMiddleCircle(QPainter &painter, int r) {
    //画小圆
    painter.setPen(QPen(Qt::white, 3));
    painter.setBrush(Qt::NoBrush);
    painter.drawEllipse(QPoint(0, 0), r, r);
    painter.save();//保存初始坐标系位置
}
void Widget::drawMiddleValue(QPainter &painter) {
    painter.setFont(QFont("楷体", 30));
    painter.drawText(QRect(-60, -60, 120, 120), Qt::AlignCenter, QString::number(currentValue));
}
void Widget::drawGradation(QPainter &painter) {
    //画刻度
    painter.setFont(QFont("华文宋体", 15));
    //旋转坐标系
    painter.rotate(135);//旋转135度(相比前一次的painter位置旋转)

    int r = height() / 2 - 40; //刻度数值所在的圆周的半径
    double x, y; //以中心为原点，水平向右为x轴正方向建立平面直角坐标系
    double targetX;//绘制刻度值时的目标坐标原点x坐标(本坐标系),只需往x方向移动
    double angle_move;//指针相对于零刻度移动的角度
    //angle_move=angle*i%45 sin(angle_move)=y/r cos(angle_move)=x/r
    for (int i = 0; i <= 60; i++) {
        if (i % 5 == 0) {
            angle_move = fmod(angle * i, 45.0);
            x = cos(angle_move) * r;
            y = sin(angle_move) * r;
            targetX = sqrt(x * x + y * y);
            painter.save();//保存原位置
            painter.translate(targetX, 0);
            painter.rotate(90);
            painter.drawText(-10, 4, QString::number(i * 4));
            painter.restore();//恢复原位置
            //画大刻度
            painter.drawLine(height() / 2 - 15, 0, height() / 2 - 1, 0);
            //painter.drawText(height() / 2 - 50, 4, QString::number(i));注意第二个参数
        } else {
            //画小刻度
            painter.drawLine(height() / 2 - 6, 0, height() / 2 - 1, 0);
        }
        painter.rotate(angle);
    }
}
void Widget::drawPointer(QPainter &painter, int r) {
    painter.restore();//坐标轴回到初始位置
    painter.save();
    painter.rotate(135 + currentValue * angle);
    painter.drawLine(r, 0, r * 4 - 62, 0);
}
void Widget::draw_Pie(QPainter &painter, int r) {
    painter.restore();
    painter.setPen(Qt::NoPen);
    painter.setBrush(QColor(255, 0, 0, 150));
    painter.drawPie(QRect(-r / 2 + 60, -r / 2 + 60, r - 120, r - 120), (- 135) * 16, -angle * currentValue * 16);
}
void Widget::paintEvent(QPaintEvent *event) {
    QPainter painter(this);
    //初始化画布
    initCanvas(painter);
    //画小圆
    drawMiddleCircle(painter, height() / 8);
    //画值
    drawMiddleValue(painter);

    //画刻度
    drawGradation(painter);
    //画指针

    drawPointer(painter, height() / 8);
    //画扇形
    draw_Pie(painter, height());
}
