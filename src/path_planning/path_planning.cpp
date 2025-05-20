#include "path_planning.h"


PathPlanning::PathPlanning(QWidget *widget, MapModel *map_model): QWidget(widget) {
    this->QWidget::setVisible(false);
    map_model_ = map_model;
    goal_nodes_model = new QStandardItemModel(this);
    list_view_ = new QListView(this);
    list_view_->setFont(QFont("Helvetica", 12, QFont::Bold));
    result_widget_ = new ResultWidget(this);
    list_view_->setModel(goal_nodes_model);
    add_button_ = new QPushButton("add", this);
    remove_button_ = new QPushButton("remove", this);
    combo_box_ = new QComboBox(this);
    station_model_ = new QStandardItemModel(this);
    whole_distance_label_ = new QLabel("whole distance :", this);

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
    this->layout()->addWidget(whole_distance_label_);
    updateWholeDistanceLabel();
    connect(add_button_, &QPushButton::clicked, this, &PathPlanning::addStation);
    connect(remove_button_, &QPushButton::clicked, this, &PathPlanning::removeStation);
    connect(remove_button_, &QPushButton::clicked, this, [this] { emit MapViewCancelHighlightPath(); });
}

void PathPlanning::addStation() {
    if (combo_box_->currentText().isEmpty()) {
        return;
    }
    auto *item = new QStandardItem(combo_box_->currentText());
    goal_nodes_model->appendRow(item);
    list_view_->update();
    solve();
    updateWholeDistanceLabel();
}

void PathPlanning::removeStation() {
    if (distance_stack_.empty()) {
        return;
    }
    whole_distance_-=distance_stack_.pop();
    updateWholeDistanceLabel();
    goal_nodes_model->removeRow(goal_nodes_model->rowCount() - 1);
    list_view_->update();
    result_widget_->removeResult();
}

void PathPlanning::solve() {
    long long start = 0;
    long long end = 0;
    if (goal_nodes_model->rowCount() == 1) {
        return;
    }
    auto row_count = goal_nodes_model->rowCount();
    start = goal_nodes_model->data(goal_nodes_model->index(row_count-2, 0)).toLongLong();
    end = goal_nodes_model->data(goal_nodes_model->index(row_count-1, 0)).toLongLong();
    map_model_->graph_.calculateShortestPaths(start);

    result_widget_->addResult(map_model_->graph_.formatResult(end));
    distance_stack_.push(map_model_->graph_.getDistance(end));
    whole_distance_ += distance_stack_.top();
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

void PathPlanning::updateWholeDistanceLabel() {
    whole_distance_label_->setText("whole distance: " + QString::number(this->whole_distance_));
}
