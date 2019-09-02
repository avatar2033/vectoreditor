#include "renderarea.h"

#include <QGraphicsSceneMouseEvent>
#include <QGraphicsEllipseItem>

#include "polyline.h"

RenderArea::RenderArea(QObject *parent) : QGraphicsScene(parent)
{
    currentItem = nullptr;
    selectedTool = Tool::Selection;
    currentPen = new QPen();

    updateTimer = new QTimer();
    connect(updateTimer, &QTimer::timeout,
            this, &RenderArea::renderScene);
    updateTimer->start(5);
}

RenderArea::~RenderArea()
{
    delete currentItem;
}

/**
 * @brief Установка текущего выбранного инструмента
 * @param tool
 */
void RenderArea::setSelectedTool(RenderArea::Tool tool)
{
    selectedTool = tool;
}

/**
 * @brief Установка цвета кисти для рисования
 * @param color
 */
void RenderArea::setColor(const QColor &color)
{
    currentPen->setColor(color);
    QList<QGraphicsItem *> selectedItemsOnScene = selectedItems();
    foreach (QGraphicsItem* selectedItem, selectedItemsOnScene) {
        Polyline *polyline = qgraphicsitem_cast<Polyline*>(selectedItem);
        if (!polyline)
            continue;

        QPen pen(color, currentPen->width());
        polyline->setPen(pen);
    }
}

/**
 * @brief Установка ширины линии для рисования
 * @param width
 */
void RenderArea::setLineWidth(const int &width)
{
    currentPen->setWidth(width);
    QList<QGraphicsItem *> selectedItemsOnScene = selectedItems();
    foreach (QGraphicsItem* selectedItem, selectedItemsOnScene) {
        Polyline *polyline = qgraphicsitem_cast<Polyline*>(selectedItem);
        if (!polyline)
            continue;

        QPen pen(currentPen->color(), width);
        polyline->setPen(pen);
    }
}

/**
 * @brief Обработчик кликов мыши на графической сцене
 * @param mouseEvent
 */
void RenderArea::mousePressEvent(QGraphicsSceneMouseEvent *mouseEvent)
{
    if (mouseEvent->button() & Qt::LeftButton) {
        switch (selectedTool) {
        case Tool::Polyline: {
            Polyline *polyline = new Polyline();
            polyline->setPen(*currentPen);
            Vertex *startPoint = new Vertex(mouseEvent->scenePos());
            startPoint->setParentItem(polyline);
            connect(startPoint, &Vertex::moved,
                    polyline, &Polyline::moveVertex);
            polyline->addVertex(startPoint);
            currentItem = polyline;
            addItem(currentItem);
            selectedTool = Tool::PolylineEdit;
            break;
        }
        case Tool::PolylineEdit: {
            Vertex *point = new Vertex(mouseEvent->scenePos());
            connect(point, &Vertex::moved,
                    qgraphicsitem_cast<Polyline*>(currentItem), &Polyline::moveVertex);
            point->setParentItem(currentItem);
            qgraphicsitem_cast<Polyline*>(currentItem)->addVertex(point);
            break;
        }
        case Tool::Delete: {
            QGraphicsItem *item = itemAt(mouseEvent->scenePos(), QTransform());
            if (item) {
                removeItem(item);
                delete item;
            }
            break;
        }
        default:
            break;
        }
    }

    if (mouseEvent->button() & Qt::RightButton) {
        setSelectedTool(Tool::Selection);
    }

    QGraphicsScene::mousePressEvent(mouseEvent);
}

/**
 * @brief Обработчик движения мыши над графической сценой
 * @param mouseEvent
 */
void RenderArea::mouseMoveEvent(QGraphicsSceneMouseEvent *mouseEvent)
{
    QString string = QString("%1, %2")
                           .arg(mouseEvent->scenePos().x())
                           .arg(mouseEvent->scenePos().y());
    emit coordChanged(string);

    QGraphicsScene::mouseMoveEvent(mouseEvent);
}

/**
 * @brief Обработчик нажатия клавиш на графической сцене
 * @param keyEvent
 */
void RenderArea::keyPressEvent(QKeyEvent *keyEvent)
{
    switch (keyEvent->key()) {
    case Qt::Key_Escape: {
        selectedTool = Tool::Selection;
        break;
    }
    case Qt::Key_Delete: {
        QList<QGraphicsItem *> selectedItemsOnScene = selectedItems();
        foreach (QGraphicsItem* selectedItem, selectedItemsOnScene) {
            removeItem(selectedItem);
            delete selectedItem;
        }
    }
    }
}

/**
 * @brief Слот для перерисовки сцены
 * @details Соединён с таймером, по которому сцена перерисовывается
 */
void RenderArea::renderScene()
{
    update(0,0,this->width(), this->height());
}
