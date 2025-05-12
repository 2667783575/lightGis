//
// Created by eric on 4/18/25.
//

#include "property_view.h"

PropertyView::PropertyView(MapModel *map_model,QWidget *parent): QWidget(parent),map_model_(map_model){
    setVisible(false);
    this->setWindowTitle("Info Panel");
    model_ = new QStandardItemModel(this);
    property_view_ = new QTableView(this);
    property_view_->setFont(QFont());
    property_view_->setModel(model_);
    property_view_->setMinimumSize(400,500);
    property_view_->setSizePolicy(QSizePolicy::Preferred,QSizePolicy::Preferred);
    property_view_->setSelectionMode(QAbstractItemView::NoSelection);
    property_view_->setSelectionBehavior(QAbstractItemView::SelectItems);

}

void PropertyView::display(const long long id) const {
    model_->clear();
    model_->setHeaderData(0, Qt::Horizontal, QString("Property"));
    model_->setHeaderData(1, Qt::Horizontal, QString("Value"));
    model_->setHorizontalHeaderLabels({"Property", "Value"});
    property_view_->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    property_view_->verticalHeader()->setVisible(false);
    model_->setColumnCount(2);

    qDebug()<<"id:"<<id;
        if ((*map_model_->id_to_item())[id]->getType() == GeoItem::Node) {
            auto item = reinterpret_cast<GeoNode *>((*map_model_->id_to_item())[id]);
            addProperty("Name",item->name());
            item->getType();
            if (item->getType() == GeoItem::Node) {
                addProperty("Type","Node");
                QString node_level;
                if (item->level() == GeoNode::Province) {
                    node_level = "Province";
                }else if (item->level() == GeoNode::City) {
                    node_level = "City";
                }else if (item->level() == GeoNode::Country) {
                    node_level = "Country";
                }

                addProperty("level",node_level);
                addProperty("acroutes",QString::number(item->acroutes()));
                addProperty("adcode",QString::number(item->adcode()));
                addProperty("subFeatureIndex",QString::number(item->sub_feature_index()));
                addProperty("center",QString("(%1,%2)").arg(item->center().longitude()).arg(item->center().latitude()));
                addProperty("centroid",QString("(%1,%2)").arg(item->centroid().longitude()).arg(item->centroid().latitude()));
                addProperty("childrenNum",QString::number(item->childrenNum()));
                property_view_->update();
            }
        }

}

void PropertyView::addProperty(const QString &name, const QString &value) const{
    const int row = model_->rowCount();
    model_->insertRow(row);
    model_->setData(model_->index(row,0),name);
    model_->setData(model_->index(row,1),value);

}

