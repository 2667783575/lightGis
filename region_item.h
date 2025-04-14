//
// Created by eric on 4/14/25.
//

#ifndef REGION_ITEM_H
#define REGION_ITEM_H
#include <QGeoPolygon>
#include <QGraphicsPathItem>

class RegionItem final :public QGraphicsPathItem {
public:
    RegionItem()=default;

    explicit RegionItem(const QGeoPolygon& polygon);
    static QSize size_;
    static void setSize(const QSize size) {
        size_ = size;
    }
protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
};



#endif //REGION_ITEM_H
