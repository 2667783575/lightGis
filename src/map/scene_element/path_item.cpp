//
// Created by eric on 5/2/25.
//

#include "../scene_element/path_item.h"

#include <qfont.h>
#include <qpen.h>


PathItem::PathItem(long long f_node, long long t_node, const QGeoPath &path, const long long id) {
    setId(id);
    auto pen = QPen(Qt::red);
    setZValue(1);
    pen.setWidthF(0.1);
    this->setPen(pen);
    // qDebug()<<size_;


    QPainterPath painter;
    if (path.path().empty()) {
        qDebug() << "empty path";
        return;
    }
    QPointF pointF = QWebMercator::coordToMercator(path.path()[0]).toPointF();
    pointF.setX(pointF.x() * size_.width());
    pointF.setY(pointF.y() * size_.height());
    painter.moveTo(pointF);



    for (const auto &coord: path.path()) {
        QPointF p = QWebMercator::coordToMercator(coord).toPointF();
        p.setX(p.x() * size_.width());
        p.setY(p.y() * size_.height());
        painter.lineTo(p);
    }


    this->setPath(painter);
}

void PathItem::mousePressEvent(QGraphicsSceneMouseEvent *event) {
    QGraphicsPathItem::mousePressEvent(event);
    emit wakeUp(id());
    event->accept();
}

void PathItem::setHighlight(bool hl) {
    QPen pen;
    if (hl) {
        pen.setColor(QColorConstants::Svg::mediumspringgreen);
        pen.setWidthF(0.5);
    } else {
        pen = QPen(QColorConstants::Svg::red);
        pen.setWidthF(0.1);
    }
    this->setPen(pen);
}
