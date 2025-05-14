//
// Created by eric on 4/14/25.
//

#ifndef REGION_ITEM_H
#define REGION_ITEM_H
#include <QGeoPolygon>
#include <QGraphicsPathItem>

class RegionItem final:public QObject,public QGraphicsPathItem {
    Q_OBJECT
public:
    [[nodiscard]] long long id() const {
        return id_;

    }

    void setId(const long long id) {
        id_ = id;
    }

    RegionItem()=default;

    explicit RegionItem(const QGeoPolygon& polygon,long long id);
    static QSize size_;
    static void setSize(const QSize size) {
        size_ = size;
        size_.setWidth(size.height());
    }
    long long id_{};
protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;


signals:
    void wakeUp(long long id);
};



#endif //REGION_ITEM_H
