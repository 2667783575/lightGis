//
// Created by eric on 4/27/25.
//

#include "path_planning.h"

PathPlanning::PathPlanning(QWidget *parent,MapModel * map_model):QWidget(parent) {
    setVisible(false);
    map_model_ = map_model;
    nodes_model_ = new NodesModel(this);
    list_view_ = new ListView(this,nodes_model_,map_model_);
    nodes_model_ = new NodesModel(this);

}
