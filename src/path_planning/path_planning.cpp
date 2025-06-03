#include "path_planning.h"

#include <QInputDialog>

#include "path_optimizationUDP.h"
#include "path_optimizer.h"


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
    open_property_editor_button_ = new QPushButton("open property editor", this);
    QVector<double> distances(map_model_->getData().size(), 0);
    auto change_object_layout = new QHBoxLayout(this);
    property_editor_ = new PropertyEditor(this);
    change_object_button_ = new QPushButton("change object", this);
    current_object_label_ = new QLabel("Current Object: " + current_object_, this);
    goal_nodes_model->insertColumn(0);
    combo_box_->setModel(station_model_);
    combo_box_->setModelColumn(0);
    station_model_->insertColumn(0);
    change_object_widget_ = new QWidget(this);
    change_object_widget_->setLayout(change_object_layout);
    change_object_layout->addWidget(change_object_button_);
    change_object_layout->addWidget(current_object_label_);
    expr_function_ = new ExprFunction("distance", {"distance"});
    this->setLayout(new QVBoxLayout(this));
    this->layout()->addWidget(open_property_editor_button_);
    this->layout()->addWidget(change_object_widget_);
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
    connect(open_property_editor_button_, &QPushButton::clicked, this, [this]() { property_editor_->show(); });
    connect(map_model_, &MapModel::updateMapCall, this, [this]() {
        property_editor_->updateDistances(map_model_->getDistances());
    });
    connect(property_editor_, &PropertyEditor::updated, this, &PathPlanning::registerGraph);
    connect(change_object_button_, &QPushButton::clicked, this, [this]() {
        QString function = QInputDialog::getText(this, "Input Expression", "请输入表达式: ", QLineEdit::Normal, "distance");
        if (!expr_function_->changeFunction(function, property_editor_->properties())) {
            return;
        }
        // qDebug() << "change Function: ";
        // qDebug() << function;
        // qDebug() << property_editor_->properties();
        registerGraph();
        current_object_ = function.trimmed();
        current_object_label_->setText(current_object_);
    });
}

void PathPlanning::loadHash() {
    vertex_to_edge_.clear();
    edge_to_vertex_.clear();
    for (auto &i: map_model_->getData()) {
        if (i->getType() == GeoItem::Road) {
            const auto access = reinterpret_cast<GeoRoad * const &>(i);
            vertex_to_edge_[{access->fnode(), access->tnode()}] = access->id();
            edge_to_vertex_[access->id()] = {access->fnode(), access->tnode()};
        }
    }
}

void PathPlanning::registerGraph() {
    graph_.clear();
    loadHash();
    std::map<long long, std::vector<double> > edge_data = property_editor_->data();
    for (auto &i: edge_to_vertex_) {
        // for ( auto &j : edge_data[i.first]) {
        //     qDebug() << j;
        // }
        double weight = (*expr_function_)(edge_data[i.first]);
        // qDebug() << i.first << "weigh: " << weight;
        // qDebug() << i.second.size();
        // qDebug() <<*(++i.second.begin()) << "  " <<  *i.second.begin() <<weight  ;
        // qDebug() << *i.second.begin() << "  " << *(++i.second.begin()) <<weight  ;
        graph_.addEdge(*i.second.begin(), *(++i.second.begin()), weight);
        graph_.addEdge(*(++i.second.begin()), *i.second.begin(), weight);
    }
}

void PathPlanning::addStation() {
    if (combo_box_->currentText().isEmpty()) {
        return;
    }
    auto *item = new QStandardItem(combo_box_->currentText());
    goal_nodes_model->appendRow(item);
    list_view_->update();
    if (goal_nodes_model->rowCount() == 1) {
        return;
    }
    auto start = goal_nodes_model->data(goal_nodes_model->index(goal_nodes_model->rowCount() - 2, 0)).toLongLong();
    auto end = goal_nodes_model->data(goal_nodes_model->index(goal_nodes_model->rowCount() - 1, 0)).toLongLong();
    graph_.calculateShortestPaths(start);
    QString result_string = graph_.formatResult(end);
    result_widget_->addResult(result_string);
    double result = graph_.getDistance(end);
    distance_stack_.push(result);
    whole_distance_ += result;
    updateWholeDistanceLabel();
    emit MapViewHighlightPath(graph_.getPath(end));
}

void PathPlanning::removeStation() {
    list_view_->update();
    goal_nodes_model->removeRow(goal_nodes_model->rowCount() - 1);
    result_widget_->removeResult();

    if (distance_stack_.empty()) {
        return;
    }
    whole_distance_ -= distance_stack_.pop();
    updateWholeDistanceLabel();
    emit MapViewCancelHighlightPath();
}

// void PathPlanning::test_() {
//     loadHash();
//     std::cout << "hash" << edge_to_vertex_.size() << std::endl;
//     // for (auto &i : property_editor_->data()) {
//     //     for (auto &j : i) {
//     //         std::cout << j << " ";
//     //
//     //     }
//     // }
//     expr_function_ = new ExprFunction("distance", {"distance"});
//     std::map<long long,std::vector<double>> data_map;
//     for (auto &i :property_editor_->data()) {
//         if (i.second.empty()) {
//             continue;
//         }
//
//         data_map[i.first] = i.second;
//     }
//     std::cout << "attri" << std::endl;
//     for (auto i : data_map) {
//         std::cout << i.first << ": "<< std::endl;
//         for (auto j : i.second) {
//             std::cout << j << " ";
//         }
//     }
//     std::cout << "edge to vertex" << std::endl;
//     for (auto i : edge_to_vertex_) {
//         std::cout << i.first << " ";
//         for (auto j : i.second) {
//             std::cout << j << " ";
//         }
//     }
//     Graph graph{data_map, edge_to_vertex_, vertex_to_edge_, 66, 76};
//     PathOptimizer optimizer(graph, [](const std::vector<double> &x) { return x[0]; });
//
//     auto best_path = optimizer.optimize();
//     for (auto i: best_path) {
//         for (auto s: edge_to_vertex_[i]) {
//             qDebug() << s << " ";
//         }
//         // if (edge_to_vertex_[i].find(106) != edge_to_vertex_[i].end()) {
//         //     break;
//         // }
//     }
// }

void PathPlanning::updateComboBoxModel() {
    // qDebug() << "station model";
    // qDebug() << "station model Station SIZE:";
    // qDebug() << map_model_   ->stations_.size();
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
