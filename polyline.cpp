#include "polyline.h"

#include <QCursor>
#include <QDebug>

Polyline::Polyline(QObject* parent) : QObject(parent), QGraphicsPathItem ()
{
    setFlags(ItemIsSelectable);

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
 * @brief Перерисовка ломаной
 */
void Polyline::updatePath()
{
    QPainterPath newPath;

    bool first = true;

    foreach (auto vertex, vertexes) {
        if (first) {
            newPath.moveTo(vertex->scenePosition);
            first = false;
        } else {
            newPath.lineTo(vertex->scenePosition);
        }
    }
    setPath(newPath);
}

/**
 * @brief Обработчик события клика мыши
 * @param mouseEvent
 */
void Polyline::mousePressEvent(QGraphicsSceneMouseEvent *mouseEvent)
{
    scenePosition = mouseEvent->pos();
    QGraphicsPathItem::mousePressEvent(mouseEvent);
}

/**
 * @brief Обработчик события отпускания кнопки мыши
 * @param mouseEvent
 */
void Polyline::mouseReleaseEvent(QGraphicsSceneMouseEvent *mouseEvent)
{
    QGraphicsPathItem::mouseReleaseEvent(mouseEvent);
}

/**
 * @brief Слот для перемещения вершины и обновления пути ломаной
 * @param Vertex* vertex Указатель на передвигаемую точку ломаной
 * @param qreal dx Смещение вершины по оси X
 * @param qreal dy Смещение вершины по оси Y
 */
void Polyline::moveVertex(Vertex *vertex, qreal dx, qreal dy)
{
    QPainterPath painterPath = path();
    for (int i = 0; i < painterPath.elementCount(); i++){
        if (vertexes.at(i) == vertex) {
            QPointF point = painterPath.elementAt(i);
            painterPath.setElementPositionAt(i, point.x() + dx, point.y() + dy);
        }
    }
    setPath(painterPath);
}

/**
 * @brief Обработчик события удаления вершины ломаной
 * @details Обновляет объект ломаной и перерисовывает путь.
 * @warning Не удаляет указатель. Только удаляет его из списка вершин и
 * вызывает метод перерисовки пути.
 * @param Vertex* v Указатель на вершину в ломаной
 */
void Polyline::onVertexDeleted(Vertex *v)
{
    for (int i = 0; i < vertexes.size(); i++) {
        if (vertexes.at(i) == v) {
            vertexes.remove(i);
            updatePath();
        }
    }
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
        previousPoint->setX(vertex->scenePosition.x());
        previousPoint->setY(vertex->scenePosition.y());
        painterPath.moveTo(*previousPoint);
    } else {
        *previousPoint = vertexes.last()->scenePos();
        painterPath = path();
        painterPath.lineTo(vertex->scenePosition);
    }
    setPath(painterPath);

    vertexes.append(vertex);
    scenePosition = sceneBoundingRect().center();
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
    setPos(mapToParent(mouseEvent->pos() - scenePosition));
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
    QPointF clickPos = event->pos();
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
            connect(newVertex, &Vertex::deleted,
                    this, &Polyline::onVertexDeleted);
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
}




