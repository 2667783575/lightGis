#ifndef PATH_PLANNING_H
#define PATH_PLANNING_H
#include <QComboBox>
#include <QPushButton>
#include <QStandardItemModel>
#include <QWidget>
#include "list_view.h"
#include "nodes_model.h"
#include "result_widget.h"

class PathPlanning:public QWidget {
  Q_OBJECT
public:
  PathPlanning(QWidget *widget,MapModel *map_model);
  ~PathPlanning() override;
private:
  ResultWidget *result_widget_;
  ListView  *list_view_;
  QPushButton *add_button_;
  QPushButton *remove_button_;
  QComboBox *combo_box_;
  MapModel *map_model_;
  QStandardItemModel *station_model_;
  QStandardItemModel *goal_nodes_model;
  public slots:
  void updateComboBoxModel();
  void addStation();
  void removeStation();
  void solve();
};

#endif