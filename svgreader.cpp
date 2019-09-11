#include "svgreader.h"

#include <QDomDocument>
#include <QFile>

SvgReader::SvgReader(QObject *parent) : QObject(parent)
{

}

/**
 * @brief Функция для парсинга SVG файла и получения из него объектов ломаных
 * @param filename Путь к файлу
 * @return
 */
QList<Polyline *> SvgReader::getPolylines(const QString filename)
{
    QList<Polyline *> polylineList;

    QDomDocument doc;
    QFile file(filename);
    if (!file.open(QIODevice::ReadOnly) || !doc.setContent(&file))
        return polylineList;

    QDomNodeList pList = doc.elementsByTagName("g");
    for (int i = 0; i < pList.size(); i++) {
        QDomNode pNode = pList.item(i);
        QDomElement pathElement = pNode.firstChildElement("path");
        if (pathElement.isNull()){
            continue;
        } else {
            Polyline *polyline = new Polyline();
            auto pElement = pNode.toElement();

            polyline->setBrush(QBrush(Qt::transparent));

            QColor strokeColor(pElement.attribute("stroke", "#000000"));
            strokeColor.setAlphaF(pElement.attribute("stroke-opacity").toDouble());
            polyline->setPen(QPen(strokeColor, pElement.attribute("stroke-width", "0").toInt()));

            QPainterPath path;
            QStringList listDotes = pathElement.attribute("d").split(" ");
            QString first = listDotes.at(0);
            QStringList firstElement = first.replace(QString("M"),QString("")).split(",");
            Vertex *firstVertex = new Vertex(QPointF(firstElement.at(0).toInt(),firstElement.at(1).toInt()));
            firstVertex->setParentItem(polyline);
            connect(firstVertex, &Vertex::moved,
                    polyline, &Polyline::moveVertex);
            polyline->addVertex(firstVertex);
            path.moveTo(firstElement.at(0).toInt(),firstElement.at(1).toInt());
            for(int i = 1; i < listDotes.length(); i++){
                QString other = listDotes.at(i);
                QStringList dot = other.replace(QString("L"),QString("")).split(",");
                path.lineTo(dot.at(0).toInt(),dot.at(1).toInt());
                Vertex *vertex = new Vertex(QPointF(dot.at(0).toInt(),dot.at(1).toInt()));
                vertex->setParentItem(polyline);
                connect(vertex, &Vertex::moved,
                        polyline, &Polyline::moveVertex);
                connect(vertex, &Vertex::deleted,
                        polyline, &Polyline::onVertexDeleted);
                polyline->addVertex(vertex);

            }
            polyline->scenePosition = polyline->sceneBoundingRect().center();
            polyline->setPath(path);
            polylineList.append(polyline);
        }
    }
    file.close();
    return polylineList;
}

/**
 * @brief Функция для парсинга SVG файла и получения из него размера сцены
 * @param filename Путь к файлу
 * @return
 */
QRectF SvgReader::getSizes(const QString filename)
{
    QDomDocument doc;
    QFile file(filename);
    if (!file.open(QIODevice::ReadOnly) || !doc.setContent(&file))
        return QRectF(0,0,200,200);

    QDomNodeList list = doc.elementsByTagName("svg");
    if(list.length() > 0) {
        auto svgElement = list.item(0).toElement();
        auto parameters = svgElement.attribute("viewBox").split(" ");
        return QRectF(parameters.at(0).toInt(),
                      parameters.at(1).toInt(),
                      parameters.at(2).toInt(),
                      parameters.at(3).toInt());
    }
    return QRectF(0,0,200,200);
}
