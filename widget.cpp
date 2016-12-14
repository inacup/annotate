#include "widget.h"
#include "ui_widget.h"


Widget::Widget(QWidget *parent) : QWidget(parent), ui(new Ui::Widget) {
    ui->setupUi(this);
    Widget::setMouseTracking(true);
    capture();
    pen.setWidth(5);
    pen.setColor(QColor(255,0,0));
    state = STATE_NOP;
    new_state = STATE_DRAW;
    scale = 1.0f;
    scalerate = 0.15f;

    scale_startx = 0;
    scale_starty = 0;
    scale_width = pixmap.width();
    scale_height = pixmap.height();

    sb = SOLID_SCREENSHOT;

    label = "";
}

Widget::~Widget() {
    delete ui;
}

void Widget::capture() {
    QScreen *scr = QApplication::primaryScreen();
    pixmap = scr->grabWindow(0);
}

void Widget::paintEvent(QPaintEvent *event) {
    QPainter painter;
    QPen titlepen;

    painter.begin(this);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.setRenderHint(QPainter::SmoothPixmapTransform);
    painter.setRenderHint(QPainter::TextAntialiasing);

    switch (sb) {
    case SOLID_SCREENSHOT:
        painter.drawPixmap(0, 0, pixmap.width(), pixmap.height(), pixmap, scale_startx, scale_starty, scale_width, scale_height);
        break;
    case SOLID_BLACK:
        painter.fillRect(0, 0, pixmap.width(), pixmap.height(), QColor(0,0,0));
        break;
    case SOLID_WHITE:
        painter.fillRect(0, 0, pixmap.width(), pixmap.height(), QColor(255,255,255));
        break;
    }

    //painter.drawLine(0, 0, pixmap.width(), pixmap.height());
    //painter.drawLine(0, pixmap.height(), pixmap.width(), 0);

    painter.setPen(pen);

    for (int i=0; i<listline.count(); i++) {
        painter.setPen(listline.at(i).p);
        //painter.drawLine(listline.at(i).s.x(), listline.at(i).s.y(), listline.at(i).e.x(), listline.at(i).e.y());
        painter.drawLine((listline.at(i).s.x() - scale_startx)*scale, (listline.at(i).s.y() - scale_starty)*scale, (listline.at(i).e.x() - scale_startx)*scale,(listline.at(i).e.y() - scale_starty)*scale);
    }

    if (state == STATE_DRAWLINES) {
        painter.setPen(pen);
        painter.drawLine(start_click.x(), start_click.y(), end_click.x(), end_click.y());
    }

    if (label != "") {
        titlepen.setColor(QColor(150, 75, 0));
        painter.setPen(titlepen);
        painter.setFont(QFont("Arial", 16, QFont::Bold));
        painter.fillRect(5, 5, 170, 30, QColor(255,255,0));
        painter.drawText(5, 5, 170, 30, Qt::AlignCenter, label);
    }

    painter.end();
}

void Widget::mousePressEvent(QMouseEvent *event) {
    if (state == STATE_NOP) {
        state = new_state;
        start_click = event->pos();
        end_click = event->pos();
    }
}

void Widget::add_line(QPoint point) {
    myline l;

    l.s = start_click;
    l.s.setX(l.s.x() / scale + scale_startx);
    l.s.setY(l.s.y() / scale + scale_starty);
    l.e = point;
    l.e.setX(l.e.x() / scale + scale_startx);
    l.e.setY(l.e.y() / scale + scale_starty);
    pen.setCapStyle(Qt::RoundCap);
    l.p = pen;
    listline.append(l);
}

void Widget::mouseReleaseEvent(QMouseEvent *event) {
    if (state == STATE_DRAWLINES) {
        add_line(event->pos());
    }
    state = STATE_NOP;
    update();
}

void Widget::mouseMoveEvent(QMouseEvent *event) {
    switch (state) {
    case STATE_DRAWLINES:
        end_click = event->pos();
        break;
    case STATE_DRAW:
        add_line(event->pos());
        start_click  = event->pos();
        break;
    case STATE_PAN:
        scale_startx = (event->pos().x() * scale - event->pos().x()) / scale;
        scale_starty = (event->pos().y() * scale - event->pos().y()) / scale;
        qDebug("Panning....");
        break;
    default:
        break;
    }
    update();
}

void Widget::wheelEvent(QWheelEvent *event) {
    if (sb == SOLID_SCREENSHOT) {
    if (abs(event->delta()) > 50) {
        if (event->delta() >= 0) {
            scale += scalerate;
        } else {
            scale -= scalerate;
            if (scale < 1.0f) scale = 1.0f;
        }
        qDebug("Scale: %f", scale);
    }

    if (scale > 1.0f) {
        scale_width = pixmap.width() / scale;
        scale_height = pixmap.height() / scale;
        scale_startx = (event->pos().x() * scale - event->pos().x()) / scale;
        scale_starty = (event->pos().y() * scale - event->pos().y()) / scale;
    } else {
        scale_startx = 0;
        scale_starty = 0;
        scale_width = pixmap.width();
        scale_height = pixmap.height();
    }
    }

    update();
}

void Widget::keyPressEvent(QKeyEvent *event) {

    switch (event->key()) {
    case Qt::Key_Escape:
        QApplication::quit();
        break;
    case Qt::Key_L:
        new_state = STATE_DRAWLINES;
        break;
    case Qt::Key_D:
        new_state = STATE_DRAW;
        break;
    case Qt::Key_B:
        pen.setColor(QColor(0, 0, 255));
        break;
    case Qt::Key_R:
        pen.setColor(QColor(255, 0, 0));
        break;
    case Qt::Key_G:
        pen.setColor(QColor(0, 255, 0));
        break;
    case Qt::Key_1:
        pen.setWidth(1);
        break;
    case Qt::Key_2:
        pen.setWidth(2);
        break;
    case Qt::Key_3:
        pen.setWidth(3);
        break;
    case Qt::Key_4:
        pen.setWidth(4);
        break;
    case Qt::Key_5:
        pen.setWidth(5);
        break;
    case Qt::Key_6:
        pen.setWidth(6);
        break;
    case Qt::Key_7:
        pen.setWidth(7);
        break;
    case Qt::Key_8:
        pen.setWidth(8);
        break;
    case Qt::Key_9:
        pen.setWidth(9);
        break;
    case Qt::Key_P:
        if (sb == SOLID_SCREENSHOT) {
            if (state != STATE_PAN) {
                state = STATE_PAN;
                label = "PAN MODE";
            } else {
                state = STATE_NOP;
                label = "";
            }
        }
        break;
    case Qt::Key_Delete:
        listline.clear();
        break;
    case Qt::Key_N:
        if (sb != SOLID_BLACK) {
            sb = SOLID_BLACK;
            label = "BLACK BOARD";
        } else {
            sb = SOLID_SCREENSHOT;
            label = "";
        }
        break;
    case Qt::Key_M:
        if (sb != SOLID_WHITE) {
            sb = SOLID_WHITE;
            label = "WHITE BOARD";
        } else {
            sb = SOLID_SCREENSHOT;
            label = "";
        }
        break;
    case Qt::Key_Z:
        scale = 1.0f;
        scale_startx = 0;
        scale_starty = 0;
        scale_width = pixmap.width();
        scale_height = pixmap.height();
        break;
    default:
        break;
    }
    update();
}

void Widget::keyReleaseEvent(QKeyEvent *event) {

}
