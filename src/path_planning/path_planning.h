//
// Created by eric on 4/27/25.
//

#ifndef PATH_PLANNING_H
#define PATH_PLANNING_H
#include "list_view.h"


#include <QWidget>


class PathPlanning final :public QWidget {
public:

    PathPlanning(QWidget *parent, MapModel *map_model);

private:
    ListView *list_view_;
    NodesModel *nodes_model_;
    MapModel *map_model_;
};



#endif //PATH_PLANNING_H
