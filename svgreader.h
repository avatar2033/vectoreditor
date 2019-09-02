#ifndef SVGREADER_H
#define SVGREADER_H

#include <QObject>
#include <polyline.h>

class SvgReader : public QObject
{
    Q_OBJECT
public:
    explicit SvgReader(QObject *parent = nullptr);

    static QList<Polyline*> getPolylines(const QString filename);
    static QList<QGraphicsItem *> getElements(const QString filename);
    static QRectF getSizes(const QString filename);

signals:

public slots:
};

#endif // SVGREADER_H
