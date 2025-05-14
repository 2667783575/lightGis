//
// Created by eric on 5/2/25.
//

#include "../scene_element/path_item.h"

#include <qfont.h>
#include <qpen.h>


PathItem::PathItem(long long f_node,long long t_node,const QGeoPath &path,const long long id) {
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
    pointF.setX(pointF.x()*size_.width());
    pointF.setY(pointF.y()*size_.height());
    painter.moveTo(pointF);
    for(const auto &coord :path.path()) {
        QPointF p = QWebMercator::coordToMercator(coord).toPointF();
        p.setX(p.x()*size_.width());
        p.setY(p.y()*size_.height());
        painter.lineTo(p);
    }
    QFont font;
    font.setPointSizeF(1);
    font.setBold(false);
    font.setWeight(QFont::Light);
    font.setFamily("宋体");
    painter.addText(pointF,font,QString("%1").arg(f_node));

    QPointF pointT = QWebMercator::coordToMercator(path.path()[path.path().length()-1]).toPointF();
    pointT.setX(pointT.x()*size_.width());
    pointT.setY(pointT.y()*size_.height());
    painter.moveTo(pointT);
    painter.addText(pointT,font,QString("%1").arg(t_node));

    this->setPath(painter);
}

void PathItem::mousePressEvent(QGraphicsSceneMouseEvent *event) {
    QGraphicsPathItem::mousePressEvent(event);
    emit wakeUp(id());
    event->accept();
}
