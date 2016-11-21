#pragma once

#include <QtWidgets/QWidget>
#include <QtCore/QScopedPointer>
#include <QtCore/QVector>

class QBasicTimer;
class QRectF;
class QPointF;


class Widget : public QWidget
{
public:
    Widget(QWidget *parent = 0);
    ~Widget();

protected:
    void paintEvent (QPaintEvent  * event) override;
    void timerEvent (QTimerEvent  * event) override;
    void resizeEvent(QResizeEvent * event) override;

protected:
    QScopedPointer<QBasicTimer> m_timer;

    QVector<QRectF>  m_rects;
    QVector<QPointF> m_points;
};

