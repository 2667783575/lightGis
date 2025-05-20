//
// Created by eric on 4/18/25.
//

#ifndef NODE_INFO_PANEL_H
#define NODE_INFO_PANEL_H
#include <QWidget>
#include <QListWidget>
#include <QVBoxLayout>
#include <QTableView>
#include <QHeaderView>
#include <QStandardItemModel>


#include "../map/map_element/geo_node.h"
#include "../map/map_model.h"

class PropertyView final : public QWidget {
    Q_OBJECT

public:
    explicit PropertyView(MapModel *map_model, QWidget *parent = nullptr);

    void addProperty(const QString &name, const QString &value) const;

public slots:
    void displayRegion(long long id);

public slots:
    void displayPath(long long id);

private:
    MapModel *map_model_;
    QTableView *property_view_;
    QStandardItemModel *model_;
};


#endif //NODE_INFO_PANEL_H
