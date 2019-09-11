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
    QPointF scenePosition;

    explicit Vertex(QPointF pos, qreal radius = 3, QObject *parent = nullptr);
    ~Vertex() override;

    // Определение типа для кастомного QGraphicsItem
    // Нужно для возможности применения к нему qgraphicsitem_cast
    enum { Type = UserType + 2 };
    int type() const override;

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *mouseEvent) override;
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *mouseEvent) override;
    void mouseMoveEvent(QGraphicsSceneMouseEvent *mouseEvent) override;

    void hoverEnterEvent(QGraphicsSceneHoverEvent *event) override;
    void hoverLeaveEvent(QGraphicsSceneHoverEvent *event) override;

private:
    /**
     * @brief Радиус точки-сопряжения ломаной
     */
    qreal radius;

    /**
     * @brief Вспомогательная переменная для вычисления координат курсора
     * после отпускания кнопки мыши
     */
    QPointF clickOffsetPos;

signals:
    void moved(Vertex *v, qreal xpos, qreal ypos);
    void deleted(Vertex *v);

};

#endif // VERTEX_H
