//
// Created by eric on 4/18/25.
//

#include "property_view.h"

PropertyView::PropertyView(MapModel *map_model, QWidget *parent): QWidget(parent), map_model_(map_model) {
    QWidget::setVisible(false);
    this->setWindowTitle("Info Panel");
    this->setFixedSize(400,500);
    model_ = new QStandardItemModel(this);
    property_view_ = new QTableView(this);
    property_view_->setFont(QFont());
    property_view_->setModel(model_);
    property_view_->setFixedSize(this->size());
    property_view_->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
    property_view_->setSelectionMode(QAbstractItemView::NoSelection);
    property_view_->setSelectionBehavior(QAbstractItemView::SelectItems);
}

void PropertyView::displayRegion(const long long id)  {
    model_->clear();
    model_->setHeaderData(0, Qt::Horizontal, QString("Property"));
    model_->setHeaderData(1, Qt::Horizontal, QString("Value"));
    model_->setHorizontalHeaderLabels({"Property", "Value"});
    property_view_->horizontalHeader()->setSectionResizeMode(QHeaderView::Fixed);
    property_view_->verticalHeader()->setVisible(false);
    model_->setColumnCount(2);
    property_view_->setColumnWidth(0,this->width()/2);
    property_view_->setColumnWidth(1,this->width()/2);
    property_view_->setGridStyle(Qt::PenStyle::SolidLine);
    // qDebug() << "id:" << id;
    if ((*map_model_->id_to_item())[id]->getType() == GeoItem::Node) {
        auto item = reinterpret_cast<GeoNode *>((*map_model_->id_to_item())[id]);
        addProperty("Name", item->name());
        addProperty("Type", "Node");
        QString node_level;
        if (item->level() == GeoNode::Province) {
            node_level = "Province";
        } else if (item->level() == GeoNode::City) {
            node_level = "City";
        } else if (item->level() == GeoNode::Country) {
            node_level = "Country";
        }

        addProperty("Level", node_level);
        addProperty("Acroutes", QString::number(item->acroutes()));
        addProperty("Adcode", QString::number(item->adcode()));
        addProperty("SubFeatureIndex", QString::number(item->sub_feature_index()));
        addProperty("Center", QString("(%1,%2)").arg(item->center().longitude()).arg(item->center().latitude()));
        addProperty(
            "Centroid", QString("(%1,%2)").arg(item->centroid().longitude()).arg(item->centroid().latitude()));
        addProperty("ChildrenNum", QString::number(item->childrenNum()));
        property_view_->update();
        this->setFixedSize(property_view_->size().width(),property_view_->size().height());
    }
}

void PropertyView::addProperty(const QString &name, const QString &value) const {
    const int row = model_->rowCount();
    model_->insertRow(row);
    model_->setData(model_->index(row, 0), name);
    model_->setData(model_->index(row, 1), value);
}

void PropertyView::displayPath(long long id) {
    model_->clear();
    model_->setHeaderData(0, Qt::Horizontal, QString("Property"));
    model_->setHeaderData(1, Qt::Horizontal, QString("Value"));
    model_->setHorizontalHeaderLabels({"Property", "Value"});
    property_view_->horizontalHeader()->setSectionResizeMode(QHeaderView::Fixed);
    property_view_->verticalHeader()->setVisible(false);
    model_->setColumnCount(2);
    property_view_->setColumnWidth(0,this->width()/2);
    property_view_->setColumnWidth(1,this->width()/2);
    // qDebug() << "id:" << id;
    if ((*map_model_->id_to_item())[id]->getType() == GeoItem::Road) {
        auto item = reinterpret_cast<GeoRoad *>((*map_model_->id_to_item())[id]);
        addProperty("Type", "Road");
        addProperty("Name", item->name());
        addProperty("Pinyin", item->pinyin());
        addProperty("FNode", QString::number(item->fnode()));
        addProperty("TNode", QString::number(item->tnode()));
        addProperty("Length", QString::number(item->length()));
        addProperty("Gbcode", QString::number(item->gbcode()));
        addProperty("LPoly", QString::number(item->lpoly()));
        addProperty("RPoly", QString::number(item->rpoly()));
        addProperty("Rai4m", QString::number(item->rai_4_m()));
        addProperty("Rai4mId", QString::number(item->rai_4_m_id()));

        this->setFixedSize(property_view_->size().width(),property_view_->size().height());
        property_view_->update();
    }
}
