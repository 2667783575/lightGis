//
// Created by eric on 4/14/25.
//

#include "../scene_element/region_item.h"

#include <iostream>
#include <ostream>
#include <qfont.h>
#include <QGraphicsSceneMouseEvent>
#include <private/qdoublevector2d_p.h>
#include <private/qwebmercator_p.h>

void RegionItem::mousePressEvent(QGraphicsSceneMouseEvent *event) {
    QGraphicsPathItem::mousePressEvent(event);
    emit wakeUp(id());
    event->accept();
}

RegionItem::RegionItem(const QGeoPolygon &polygon, const long long id) {
    constexpr double scale_factor = 1.0;
    constexpr double filter_factor = 0.01;
    QPolygonF polygon_f;
    bool first_time = true;
    QPointF point_pre;
    setId(id);
    // if (polygon.perimeter().isEmpty()) {
    //     return;
    // }
    QPainterPath path;

    for (const auto &coordinate: polygon.perimeter()) {
        auto point = QWebMercator::coordToMercator(coordinate).toPointF();
        point.setX(point.x() * size_.width() * scale_factor);
        point.setY(point.y() * size_.height() * scale_factor);
        if (!first_time) {
            if (std::abs(point_pre.x() - point.x()) > size_.width() * scale_factor / filter_factor ||
                std::abs(point_pre.y() - point.y()) > size_.height() * scale_factor / filter_factor) {

                continue;
            }
        }
        first_time = false;
        polygon_f << point;
        point_pre = point;
    }
    polygon_f.append(polygon_f[0]);
    path.setFillRule(Qt::OddEvenFill);
    path.addPolygon(polygon_f);
    for (int i = 0; i < polygon.holesCount(); i++) {
        const auto &hole = polygon.holePath(i);
        QList<QPointF> points;
        QPointF prev = {0, 0};
        bool first = true;
        for (const auto &point: hole) {
            auto pnt = QWebMercator::coordToMercator(point).toPointF();
            pnt.setX(pnt.x() * size_.width() * scale_factor);
            pnt.setY(pnt.y() * size_.height() * scale_factor);
            if (!first) {
                if (std::abs(prev.x() - pnt.x()) > size_.width() * scale_factor / filter_factor ||
                    std::abs(prev.y() - pnt.y()) > size_.height() * scale_factor / filter_factor) {
                    continue;
                }
            }
            first = false;
            points << pnt;
            prev = pnt;
            path.addPolygon(points);
        }
    }

    path.closeSubpath();

    this->setPath(path);
}
