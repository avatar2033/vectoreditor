#ifndef RENDERAREA_H
#define RENDERAREA_H

#include <QObject>
#include <QGraphicsScene>
#include <QGraphicsItem>
#include <QKeyEvent>
#include <QTimer>

#include <QDebug>

class RenderArea : public QGraphicsScene
{
    Q_OBJECT
public:
    enum class Tool {
        Selection = 0,
        Polyline,
        PolylineEdit,
        Delete
    };

    explicit RenderArea(QObject *parent = nullptr);
    ~RenderArea() override;

signals:
    void coordChanged(QString);

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *mouseEvent) override;
    void mouseMoveEvent(QGraphicsSceneMouseEvent *mouseEvent) override;
    void keyPressEvent(QKeyEvent *keyEvent) override;
    void removeItem(QGraphicsItem *item);

private:
    /**
     * @brief Текущий инструмент
     */
    Tool selectedTool;

    /**
     * @brief Текущий элемент для добавления на схему
     */
    QGraphicsItem *currentItem;

    /**
     * @brief Текущая настройка кисти
     */
    QPen *currentPen;

    /**
     * @brief Указатель на таймер для перерисовки сцены
     */
    QTimer *updateTimer;

    QPointF mousePosition;

public slots:
    void setSelectedTool(Tool tool);
    void setColor(const QColor &color);
    void setLineWidth(const int &width);

private slots:
    void renderScene();
};

#endif // RENDERAREA_H
