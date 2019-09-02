#ifndef POLYLINE_H
#define POLYLINE_H

#include <QObject>
#include <QGraphicsPathItem>
#include <QVector>
#include <QPainter>
#include <QJsonObject>

#include "vertex.h"

class Polyline : public QObject, public QGraphicsPathItem
{
    Q_OBJECT

public:
    /**
     * @brief Центр фигуры в системе координат сцены
     */
    QPointF scenePos;

    /**
     * @brief Массив точек-изломов ломаной линии
     */
    QVector<Vertex*> vertexes;

    explicit Polyline(QObject* parent = nullptr);
    ~Polyline() override;

    // Определение типа для кастомного QGraphicsItem
    // Нужно для возможности применения к нему qgraphicsitem_cast
    enum {Type = UserType + 1};
    int type() const override;

protected:
    void mouseMoveEvent(QGraphicsSceneMouseEvent *mouseEvent) override;
    void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event) override;

private:
    /**
     * @brief Вспомогательная точка для отрисовки линии при добавлении
     * новой вершины.
     */
    QPointF *previousPoint;

public slots:
    void moveVertex(Vertex* vertex, qreal xpos, qreal ypos);
    void addVertex(Vertex *vertex);
    void setBrush(const QBrush &brush);
};

#endif // POLYLINE_H
