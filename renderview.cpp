#include "renderview.h"


RenderView::RenderView(QGraphicsScene *scene, QWidget *parent)
    : QGraphicsView(scene, parent)
{

}

void RenderView::enterEvent(QEvent *event)
{
    setCursor(QCursor(Qt::CrossCursor));
    QGraphicsView::enterEvent(event);
}

void RenderView::leaveEvent(QEvent *event)
{
    setCursor(QCursor(Qt::ArrowCursor));
    QGraphicsView::leaveEvent(event);
}
