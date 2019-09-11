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
    QPointF scenePosition;

    /**
     * @brief Массив точек-изломов ломаной линии
     */
    QVector<Vertex*> vertexes;

    explicit Polyline(QObject *parent = nullptr);
    ~Polyline() override;

    // Определение типа для кастомного QGraphicsItem
    // Нужно для возможности применения к нему qgraphicsitem_cast
    enum {Type = UserType + 1};
    int type() const override;

    void updatePath();

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *mouseEvent) override;
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *mouseEvent) override;
    void mouseMoveEvent(QGraphicsSceneMouseEvent *mouseEvent) override;
    void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event) override;

private:
    /**
     * @brief Вспомогательная точка для отрисовки линии при добавлении
     * новой вершины.
     */
    QPointF *previousPoint;

public slots:
    void moveVertex(Vertex *vertex, qreal xpos, qreal ypos);
    void onVertexDeleted(Vertex *v);
    void addVertex(Vertex *vertex);
    void setBrush(const QBrush &brush);
};

#endif // POLYLINE_H
