#ifndef PATH_PLANNING_H
#define PATH_PLANNING_H
#include <QComboBox>
#include <QPushButton>
#include <QStandardItemModel>
#include <QWidget>
#include <qlayout.h>
#include "algorithms.h"
#include "property_editor.h"
#include "../map/map_model.h"
#include "result_widget.h"
#include "expr_function.h"
#include "path_optimizationUDP.h"


// 路径规划类，处理路径规划的ui模块
class PathPlanning : public QWidget {
    Q_OBJECT

public:
    PathPlanning(QWidget *widget, MapModel *map_model);

    ~PathPlanning() override;

    void set_stations(QSet<long long> *stations) {
        stations_ = stations;
    }

    void updateWholeDistanceLabel();

    void loadHash();
public slots:

    void registerGraph();
signals:
    void MapViewHighlightPath(QList<long long> path);

    void MapViewCancelHighlightPath();

public slots:
    void updateComboBoxModel();

    void addStation();

    void removeStation();

    // void test_();

private:
    Algorithms::Graph graph_;
    std::unordered_map<long long, std::unordered_set<long long> > edge_to_vertex_;
    std::unordered_map<std::unordered_set<long long>, long long,SetHash> vertex_to_edge_;
    ResultWidget *result_widget_;
    QListView *list_view_;
    QPushButton *add_button_;
    QPushButton *remove_button_;
    QPushButton *open_property_editor_button_;
    QComboBox *combo_box_;
    MapModel *map_model_;
    QSet<long long> *stations_;
    QStandardItemModel *station_model_;
    QStandardItemModel *goal_nodes_model;
    QLabel *whole_distance_label_;
    PropertyEditor *property_editor_;
    QPushButton *change_object_button_;
    QLabel *current_object_label_;
    QWidget *change_object_widget_;
    QString current_object_{"distance"};
    ExprFunction *expr_function_;
    QStack<qreal> distance_stack_{};
    qreal whole_distance_{0};
};

#endif
