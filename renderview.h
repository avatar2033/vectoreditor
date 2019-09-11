#ifndef RENDERVIEW_H
#define RENDERVIEW_H

#include <QGraphicsView>

class RenderView : public QGraphicsView
{
public:
    RenderView(QGraphicsScene *scene, QWidget *parent = nullptr);

protected:
    void enterEvent(QEvent *event) override;
    void leaveEvent(QEvent *event) override;
};

#endif // RENDERVIEW_H
