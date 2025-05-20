//
// Created by eric on 5/2/25.
//

#include "../scene_element/path_item.h"

#include <qfont.h>
#include <qpen.h>


PathItem::PathItem(long long f_node, long long t_node, const QGeoPath &path, const long long id) {
    setId(id);
    auto pen = QPen(Qt::black);
    QVector<qreal> pattern;
    pattern << 4.0 << 1.0;
    pen.setDashPattern(pattern);
    pen.setColor(Qt::white);
    pen.setBrush(Qt::black);
    pen.setCapStyle(Qt::FlatCap);
    pen.setJoinStyle(Qt::MiterJoin);
    setZValue(1);
    pen.setWidthF(4.0);
    pen.setWidthF(Qt::DashLine);
    pen.setCosmetic(true);
    this->setPen(pen);


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
        // QVector<qreal> pattern;
        // pattern << 4.0 << 1.0;
        // pen.setDashPattern(pattern);
        pen.setColor(QColorConstants::Svg::gold);
        pen.setBrush(QColorConstants::Svg::gold);
        setZValue(1);
        pen.setWidthF(6);
        pen.setCosmetic(true);
        this->setPen(pen);
    } else {
        QVector<qreal> pattern;
        pattern << 4.0 << 1.0;
        pen.setDashPattern(pattern);
        pen.setColor(Qt::white);
        pen.setBrush(Qt::black);
        pen.setCapStyle(Qt::FlatCap);
        pen.setJoinStyle(Qt::MiterJoin);
        pen.setWidthF(4.0);
        pen.setWidthF(Qt::DashLine);
        pen.setCosmetic(true);
        this->setPen(pen);
    }
    this->setPen(pen);
}
