//
// Created by eric on 4/18/25.
//

#ifndef NODE_INFO_PANEL_H
#define NODE_INFO_PANEL_H
#include <QWidget>

#include "../map/geo_node.h"
#include "../map/map_model.h"
#include <QListWidget>
#include <QVBoxLayout>
#include <QTableView>
#include <QHeaderView>
#include <QStandardItemModel>
class PropertyView final : public QWidget {
    Q_OBJECT

public:
    explicit PropertyView(MapModel *map_model, QWidget *parent = nullptr);
    void addProperty(const QString &name, const QString &value) const;



private:
    MapModel *map_model_;
    QTableView *property_view_;
    QStandardItemModel *model_;
public slots:
    void display(long long id) const;
};


#endif //NODE_INFO_PANEL_H
