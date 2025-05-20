//
// Created by eric on 5/2/25.
//

#ifndef PATH_ITEM_H
#define PATH_ITEM_H
#include <QGeoPath>
#include <QGraphicsPathItem>
#include <QFont>
#include <private/qdoublevector2d_p.h>
#include <private/qwebmercator_p.h>
#include <QGraphicsSceneMouseEvent>

// 路径图元类
class PathItem final : public QObject, public QGraphicsPathItem {
    Q_OBJECT

public:
    long long id_;
    static QSize size_;

    [[nodiscard]] long long id() const {
        return id_;
    }

    void setId(const long long id) {
        id_ = id;
    }

    [[nodiscard]] static QSize size() {
        return size_;
    }

    static void setSize(const QSize &size) {
        size_.setHeight(std::min(size.height(), size.width()));
        size_.setWidth(std::min(size.height(), size.width()));
    }

    PathItem(long long f_node, long long t_node, const QGeoPath &path, long long id);

    void setHighlight(bool hl);

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;

signals:
    void wakeUp(long long id);
};


#endif //PATH_ITEM_H
