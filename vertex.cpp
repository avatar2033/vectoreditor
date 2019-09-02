#include "vertex.h"

#include <QDebug>


Vertex::Vertex(QPointF pos, qreal radius, QObject *parent) : QObject(parent),
    QGraphicsEllipseItem()
{
    scenePos = pos;
    setRect(QRectF(pos.x() - radius, pos.y() - radius,
                   abs(radius * 2), abs(radius * 2)));
    setFlag(ItemIsMovable);
    setAcceptHoverEvents(true);

    this->radius = radius;
}

Vertex::~Vertex()
{
}

/**
 * @brief Обработчик наведения курсора мыши на сопряжение ломаной
 * @param event
 */
void Vertex::hoverEnterEvent(QGraphicsSceneHoverEvent *event)
{
    setBrush(Qt::red);
    QGraphicsEllipseItem::hoverEnterEvent(event);
}

/**
 * @brief Обработчик выхода курсора мыши из сопряжения ломаной
 * @param event
 */
void Vertex::hoverLeaveEvent(QGraphicsSceneHoverEvent *event)
{
    setBrush(Qt::black);
    QGraphicsEllipseItem::hoverLeaveEvent(event);
}

/**
 * @brief Обработчик перетаскивания сопряжения ломаной
 * @param mouseEvent
 */
void Vertex::mouseMoveEvent(QGraphicsSceneMouseEvent *mouseEvent)
{
    auto dx = mouseEvent->scenePos().x() - scenePos.x();
    auto dy = mouseEvent->scenePos().y() - scenePos.y();
    moveBy(dx, dy);
    scenePos = mouseEvent->scenePos();
    emit moved(this, dx, dy);
}
