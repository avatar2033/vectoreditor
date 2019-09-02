#ifndef VERTEX_H
#define VERTEX_H

#include <QObject>
#include <QGraphicsEllipseItem>
#include <QPainter>
#include <QGraphicsSceneMouseEvent>

class Vertex : public QObject, public QGraphicsEllipseItem
{
    Q_OBJECT

public:
    /**
     * @brief Центр фигуры в системе координат сцены
     */
    QPointF scenePos;

    explicit Vertex(QPointF pos, qreal radius = 3, QObject *parent = nullptr);
    ~Vertex() override;

protected:
    void mouseMoveEvent(QGraphicsSceneMouseEvent *mouseEvent) override;

    void hoverEnterEvent(QGraphicsSceneHoverEvent *event) override;
    void hoverLeaveEvent(QGraphicsSceneHoverEvent *event) override;

private:
    /**
     * @brief Радиус точки-сопряжения ломаной
     */
    qreal radius;

signals:
    void moved(Vertex *v, qreal xpos, qreal ypos);

};

#endif // VERTEX_H
