//
// Created by eric on 4/14/25.
//

#include "region_item.h"

#include <iostream>
#include <ostream>
#include <private/qdoublevector2d_p.h>
#include <private/qwebmercator_p.h>

void RegionItem::mousePressEvent(QGraphicsSceneMouseEvent *event) {
    QGraphicsPathItem::mousePressEvent(event);
}

RegionItem::RegionItem(const QGeoPolygon &polygon) {

    qDebug() << "RegionItem::RegionItem";
    QPolygonF polygon_f;
    this->setPos(0,0);
    std::for_each(polygon.perimeter().begin(), polygon.perimeter().end(), [&polygon_f, this](
              const QGeoCoordinate &item) {
                      auto point = QWebMercator::coordToMercator(item).toPointF();
                      point.setX(point.x() * size_.width());
                      point.setY(point.y() * size_.height());
                      polygon_f << point;
                  });

    QPainterPath path;
    path.addPolygon(polygon_f);
    for (int i = 0; i < polygon.holesCount(); i++) {
        const auto &hole = polygon.holePath(i);
        QList<QPointF> points;
        for (const auto &point: hole) {
            auto pnt = QWebMercator::coordToMercator(point).toPointF();
            pnt.setX(pnt.x() * size_.width());
            pnt.setY(pnt.y() * size_.height());
            points << pnt;
            path.setFillRule(Qt::OddEvenFill);
            path.addPolygon(points);
        }
    }

    this->setPath(path);
}
