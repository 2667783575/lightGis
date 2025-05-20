#ifndef PATH_PLANNING_H
#define PATH_PLANNING_H
#include <QComboBox>
#include <QPushButton>
#include <QStandardItemModel>
#include <QWidget>
#include <qlayout.h>

#include "../map/map_model.h"
#include "result_widget.h"


// 路径规划类，处理路径规划的ui模块
class PathPlanning:public QWidget {
  Q_OBJECT
public:
  PathPlanning(QWidget *widget,MapModel *map_model);
  ~PathPlanning() override;
 void set_stations(QSet<long long> *stations) {
    stations_ = stations;
  }

  void updateWholeDistanceLabel();

signals:
  void MapViewHighlightPath(QList<long long> path);

  void MapViewCancelHighlightPath();

public slots:
  void updateComboBoxModel();

  void addStation();

  void removeStation();

  void solve();

private:
  ResultWidget *result_widget_;
  QListView  *list_view_;
  QPushButton *add_button_;
  QPushButton *remove_button_;
  QComboBox *combo_box_;
  MapModel *map_model_;
  QSet<long long>* stations_;
  QStandardItemModel *station_model_;
  QStandardItemModel *goal_nodes_model;
  QLabel *whole_distance_label_;
  QStack<qreal> distance_stack_{};
  qreal whole_distance_{0};
};

#endif