#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QPixmap>
#include <QScreen>
#include <QPainter>
#include <QPen>
#include <QColor>
#include <QPoint>
#include <QMouseEvent>
#include <QList>
#include <QString>


namespace Ui {
class Widget;
}

enum States {
    STATE_NOP,
    STATE_DRAWLINES,
    STATE_DRAW,
    STATE_PAN,
};

enum SolidBackground {
    SOLID_SCREENSHOT,
    SOLID_BLACK,
    SOLID_WHITE,
};

struct myline {
    QPoint s;
    QPoint e;
    QPen p;
};

class Widget : public QWidget {
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = 0);
    ~Widget();

private:
    Ui::Widget *ui;
    QPixmap pixmap;
    void capture();
    void add_line(QPoint point);

    SolidBackground sb;

    QPen pen;
    QPoint start_click;
    QPoint end_click;
    States state;
    States new_state;

    QList<myline> listline;
    QString label;

    float scale;
    float scalerate;

    int scale_startx;
    int scale_starty;
    int scale_width;
    int scale_height;

protected:
    virtual void paintEvent(QPaintEvent *event);
    virtual void mousePressEvent(QMouseEvent *event);
    virtual void mouseReleaseEvent(QMouseEvent *event);
    virtual void mouseMoveEvent(QMouseEvent *event);
    virtual void wheelEvent(QWheelEvent *event);
    virtual void keyPressEvent(QKeyEvent *event);
    virtual void keyReleaseEvent(QKeyEvent *event);
};

#endif // WIDGET_H
