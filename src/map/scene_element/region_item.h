//
// Created by eric on 4/14/25.
//

#ifndef REGION_ITEM_H
#define REGION_ITEM_H
#include <QGeoPolygon>
#include <QGraphicsPathItem>

// 区域图元类
class RegionItem final : public QObject, public QGraphicsPathItem {
    Q_OBJECT

public:
    [[nodiscard]] long long id() const {
        return id_;
    }

    void setId(const long long id) {
        id_ = id;
    }

    RegionItem() = default;

    explicit RegionItem(const QGeoPolygon &polygon, long long id);

    static void setSize(const QSize size) {
        size_.setHeight(std::min(size.width(), size.height()));
        size_.setWidth(std::min(size.width(), size.height()));
    }

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;


signals:
    void wakeUp(long long id);

private:

    long long id_{};
    static QSize size_;
};


#endif //REGION_ITEM_H
