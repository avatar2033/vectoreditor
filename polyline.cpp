#include "polyline.h"

#include <QDebug>

Polyline::Polyline(QObject* parent) : QObject(parent), QGraphicsPathItem ()
{
    setFlags(ItemIsMovable | ItemIsSelectable);

    previousPoint = new QPointF;
}

Polyline::~Polyline()
{
    delete previousPoint;
    foreach (auto vertex, vertexes) {
        delete vertex;
    }
}

int Polyline::type() const
{
    return Type;
}

/**
 * @brief Слот для перемещения вершины и обновления пути ломаной
 * @param Vertex* vertex Указатель на передвигаемую точку ломаной
 * @param qreal dx Смещение вершины по оси X
 * @param qreal dy Смещение вершины по оси Y
 */
void Polyline::moveVertex(Vertex *vertex, qreal dx, qreal dy)
{
    QPainterPath linePath = path();
    for(int i = 0; i < linePath.elementCount(); i++){
        if(vertexes.at(i) == vertex){
            QPointF point = linePath.elementAt(i);
            linePath.setElementPositionAt(i, point.x() + dx, point.y() + dy);
            scenePos = sceneBoundingRect().center();
        }
    }
    setPath(linePath);
}

/**
 * @brief Слот для добавления новой вершины к ломаной
 * @param Vertex* vertex Указатель на новую вершину
 */
void Polyline::addVertex(Vertex *vertex)
{
    // Если в ломаной ещё нет вершин, то начинаем рисовать её от первой
    // добавленной. Иначе продолжаем рисовать последовательно.
    QPainterPath painterPath;
    if (vertexes.empty()) {
        previousPoint->setX(vertex->scenePos.x());
        previousPoint->setY(vertex->scenePos.y());
        painterPath.moveTo(*previousPoint);
    } else {
        *previousPoint = vertexes.last()->pos();
        painterPath = path();
        painterPath.lineTo(vertex->scenePos);
    }
    setPath(painterPath);

    vertexes.append(vertex);
    scenePos = sceneBoundingRect().center();
}

/**
 * @brief Слот для изменения настроек кисти ломаной
 * @param QBrush brush Ссылка на кисть
 */
void Polyline::setBrush(const QBrush &brush)
{
    QGraphicsPathItem::setBrush(brush);
}

/**
 * @brief Обработка перетаскивания ломаной по сцене
 * @param mouseEvent
 * @overload QGraphicsPathItem::mouseMoveEvent
 */
void Polyline::mouseMoveEvent(QGraphicsSceneMouseEvent *mouseEvent)
{
    auto dx = mouseEvent->scenePos().x() - scenePos.x();
    auto dy = mouseEvent->scenePos().y() - scenePos.y();
    moveBy(dx,dy);
    scenePos = mouseEvent->scenePos();

    foreach (auto vertex, vertexes) {
        vertex->scenePos.setX(vertex->scenePos.x() + dx);
        vertex->scenePos.setY(vertex->scenePos.y() + dy);
    }
}

/**
 * @brief Обработка двойного клика по ломаной.
 * @details В момент двойного клика по ломаной, в точке клика создаётся новая
 * вершина, которая делит отрезок под курсором на два.
 * @param event
 * @overload QGraphicsPathItem::mouseDoubleClickEvent
 */
void Polyline::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event)
{
    QPointF clickPos = event->scenePos();
    QLineF checkLineFirst(clickPos.x() - 5, clickPos.y() - 5, clickPos.x() + 5, clickPos.y() + 5);
    QLineF checkLineSecond(clickPos.x() + 5, clickPos.y() - 5, clickPos.x() - 5, clickPos.y() + 5);
    QPainterPath oldPath = path();
    QPainterPath newPath;
    for(int i = 0; i < oldPath.elementCount(); i++){
        QLineF checkableLine(oldPath.elementAt(i), oldPath.elementAt(i+1));
        if(checkableLine.intersect(checkLineFirst,nullptr) == 1 || checkableLine.intersect(checkLineSecond,nullptr) == 1){
            if(i == 0) {
                newPath.moveTo(oldPath.elementAt(i));
                newPath.lineTo(clickPos);
            } else {
                newPath.lineTo(oldPath.elementAt(i));
                newPath.lineTo(clickPos);
            }
            Vertex *newVertex = new Vertex(clickPos);
            newVertex->setParentItem(this);
            connect(newVertex, &Vertex::moved,
                    this, &Polyline::moveVertex);
            vertexes.insert(i + 1, newVertex);
        } else {
            if(i == 0){
                newPath.moveTo(oldPath.elementAt(i));
            } else {
                newPath.lineTo(oldPath.elementAt(i));
            }
        }
        if(i == (oldPath.elementCount() - 2)) {
            newPath.lineTo(oldPath.elementAt(i + 1));
            i++;
        }
    }
    setPath(newPath);

    QGraphicsPathItem::mouseDoubleClickEvent(event);
}




