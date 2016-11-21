#include "widget.h"


#include <QtCore/QTimerEvent>
#include <QtCore/QBasicTimer>
#include <QtCore/QRectF>
#include <QtCore/QLineF>
#include <QtCore/QPointF>
#include <QtCore/QLocale>

#include <QtGui/QPaintEvent>
#include <QtGui/QResizeEvent>
#include <QtGui/QPainter>

#include <QtGui/QVector2D>
#include <QtCore/qmath.h>


#include "particle.h"
#include "engine.h"


namespace
{
    const auto g_numCellsF = std::array<float, 2>{
        static_cast<float>(g_cellColumns), static_cast<float>(g_cellRows) };
}

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , m_timer(new QBasicTimer())
{
    //resize(640, 360);
    resize(1280, 720);

    // make widget black by default
    auto p = QPalette(palette());
    p.setColor(QPalette::Background, QColor::fromRgbF(0.04, 0.04, 0.04));

    setPalette(p);
    setAutoFillBackground(true);

    // setup rect rendering
    m_rects.resize(g_cellColumns * g_cellRows);
    m_points.resize((g_cellColumns - 1) * (g_cellRows - 1));

    // ToDo: remove
    initialize();

    // setup timer and continuous repaint
    m_timer->start(0, this);
}

Widget::~Widget()
{
}

void Widget::resizeEvent(QResizeEvent * event)
{
    const auto cellSize = QSizeF{
        static_cast<float>(event->size().width())  / g_cellColumns
      , static_cast<float>(event->size().height()) / g_cellRows };

    for(auto y = 0u; y < g_cellRows; ++y)
        for(auto x = 0u; x < g_cellColumns; ++x)
        {
            const auto i = y * g_cellColumns + x;
            m_rects[i].setRect(cellSize.width() * x, cellSize.height() * y, cellSize.width(), cellSize.height());
            // adjust for aesthetics
            //m_rects[i].adjust(7, 7, -7, -7);
            m_rects[i].adjust(3, 3, -3, -3);
        }

    // setup points for aesthetics
    for(auto y = 1u; y < g_cellRows; ++y)
        for(auto x = 1u; x < g_cellColumns; ++x)
        {
            const auto i = (y - 1) * (g_cellColumns - 1) + (x - 1);
            m_points[i].setX(cellSize.width() * x);
            m_points[i].setY(cellSize.height() * y);
        }
}

void Widget::paintEvent(QPaintEvent * /* event */)
{
    QPainter painter(this);

    painter.setPen(QPen(QColor::fromRgbF(0.32, 0.32, 0.32), 1.f));

    for(auto y = 0u; y < g_cellRows; ++y)
        for(auto x = 0u; x < g_cellColumns; ++x)
        {
            const auto i = y * g_cellColumns + x;
            const auto a = 0.08f + qMin(1.f, cells()[i] * g_numCellsF[0] * g_numCellsF[1]) * 0.24f;
            painter.setBrush(QColor::fromRgbF(a, a, a));
            painter.drawRect(m_rects[i]);
        }

    // draw points for aesthetics
    painter.setBrush(Qt::NoBrush);
    painter.setPen(QPen(QColor::fromRgbF(1.0, 1.0, 1.0), 1.f));
    painter.drawPoints(m_points);

    // draw photons
    const auto scale = QVector2D{
        static_cast<float>(width())  / g_numCellsF[0]
      , static_cast<float>(height()) / g_numCellsF[1] };

    painter.setOpacity(0.24);
    painter.setBrush(Qt::NoBrush);
    painter.setPen(QPen(Qt::white, 2.f));

    for(const auto particle : particles())
    {
        const auto p = particle->position * scale;
        painter.drawPoint(p.toPointF());
    }

    float e = 0.f;
    for(const auto particle : particles())
      e += particle->energy;
    for(const auto cell : cells())
      e += cell;

    QLocale::setDefault(QLocale::German);
    setWindowTitle(QString("#particles: %L1, energy sum: %2")
        .arg(particles().size(), 8, 10, QChar(' ')).arg(e));
}

void Widget::timerEvent(QTimerEvent * event)
{
    if(event->timerId() != m_timer->timerId())
        return QWidget::timerEvent(event);

    if(!m_timer->isActive())
        return;

    simulate();
    update();
}
