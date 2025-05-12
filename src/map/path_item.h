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

class PathItem final:public QGraphicsPathItem {
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
        size_ = size;
        size_ = size;
        size_.setWidth(size.height());
    }

    PathItem(long long f_node,long long t_node,const QGeoPath &path,long long id);
    ~PathItem() override = default;

};



#endif //PATH_ITEM_H
