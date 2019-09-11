#include "vertex.h"

#include <QCursor>
#include <QDebug>


Vertex::Vertex(QPointF pos, qreal radius, QObject *parent) : QObject(parent),
    QGraphicsEllipseItem()
{
    scenePosition = pos;
    clickOffsetPos = pos;
    setRect(QRectF(pos.x() - radius, pos.y() - radius,
                   abs(radius * 2), abs(radius * 2)));
    setAcceptHoverEvents(true);

    this->radius = radius;
}

Vertex::~Vertex()
{
}

int Vertex::type() const
{
    return Type;
}

/**
 * @brief Обработчик события клика мыши
 * @param mouseEvent
 */
void Vertex::mousePressEvent(QGraphicsSceneMouseEvent *mouseEvent)
{
    scenePosition = mouseEvent->pos();
}

/**
 * @brief Обработчик события отпускания кнопки мыши
 * @param mouseEvent
 */
void Vertex::mouseReleaseEvent(QGraphicsSceneMouseEvent *mouseEvent)
{
    Q_UNUSED(mouseEvent)

    scenePosition = clickOffsetPos;
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
    setPos(mapToParent(mouseEvent->pos() - scenePosition));
    auto dx = mouseEvent->pos().x() - scenePosition.x();
    auto dy = mouseEvent->pos().y() - scenePosition.y();
    emit moved(this, dx, dy);
    clickOffsetPos += QPointF(dx, dy);
}
