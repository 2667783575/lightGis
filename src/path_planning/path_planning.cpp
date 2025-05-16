#include "path_planning.h"

#include <qlayout.h>

PathPlanning::PathPlanning(QWidget *widget, MapModel *map_model): QWidget(widget) {
    this->setVisible(false);
    map_model_ = map_model;
    goal_nodes_model = new QStandardItemModel(this);
    list_view_ = new ListView(this);
    result_widget_ = new ResultWidget(this);
    list_view_->setModel(goal_nodes_model);
    add_button_ = new QPushButton("add", this);
    remove_button_ = new QPushButton("remove", this);
    combo_box_ = new QComboBox(this);
    station_model_ = new QStandardItemModel(this);
    goal_nodes_model->insertColumn(0);
    combo_box_->setModel(station_model_);
    combo_box_->setModelColumn(0);
    station_model_->insertColumn(0);
    this->setLayout(new QVBoxLayout(this));
    this->layout()->addWidget(list_view_);
    this->layout()->addWidget(combo_box_);
    this->layout()->addWidget(add_button_);
    this->layout()->addWidget(remove_button_);
    this->layout()->addWidget(result_widget_);
    connect(add_button_, &QPushButton::clicked, this, &PathPlanning::addStation);
    connect(remove_button_, &QPushButton::clicked, this, &PathPlanning::removeStation);
    connect(remove_button_, &QPushButton::clicked, this,[this]{emit MapViewCancelHightPath();});
}

void PathPlanning::addStation() {
    if (combo_box_->currentText().isEmpty()) {
        return;
    }
    auto * item = new  QStandardItem(combo_box_->currentText());
    goal_nodes_model->appendRow(item);
    list_view_->update();
    solve();
}

void PathPlanning::removeStation() {
    goal_nodes_model->removeRow(goal_nodes_model->rowCount()-1);
    list_view_->update();
    result_widget_->removeResult();
}

void PathPlanning::solve() {
    long long start ,end ;
    if (goal_nodes_model->rowCount() == 1) {
        return;
    }
    for (long long i = 0 ;i < goal_nodes_model->rowCount()-1;i++) {
        start = goal_nodes_model->data(goal_nodes_model->index(i,0)).toLongLong();
        end = goal_nodes_model->data(goal_nodes_model->index(i+1,0)).toLongLong();
        map_model_->graph_.calculateShortestPaths(start);
    }
    result_widget_->addResult(map_model_->graph_.formatResult(end));
    emit MapViewHighlightPath(map_model_->graph_.getPath(end));
}
void PathPlanning::updateComboBoxModel() {
    // qDebug() << "station model";
    // qDebug() << "station model Station SIZE:";
    // qDebug() << map_model_->stations_.size();
    station_model_->clear();
    std::vector<long long> stations;
    stations.assign(stations_->begin(), stations_->end());
    std::sort(stations.begin(), stations.end());
    for (const auto &station: stations) {
        QStandardItem *item = new QStandardItem(QString::number(station));
        station_model_->appendRow(item);
    }
    // emit station_model_->dataChanged(station_model_->index(0,0),station_model_->index(stations.size(),0));
    combo_box_->update();
}

PathPlanning::~PathPlanning() {
    QWidget::~QWidget();
}
