//
// Created by eric on 5/31/25.
//

#ifndef DOUBLE_DELEGATE_H
#define DOUBLE_DELEGATE_H

#include <QDoubleSpinBox>
#include <QItemDelegate>


// 自定义委托，确保只能输入double值
class DoubleDelegate : public QItemDelegate {
public:
    DoubleDelegate(QObject *parent = nullptr) : QItemDelegate(parent) {}

    QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option,
                          const QModelIndex &index) const override ;

    void setEditorData(QWidget *editor, const QModelIndex &index) const override;

    void setModelData(QWidget *editor, QAbstractItemModel *model,
                      const QModelIndex &index) const override ;

    void updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option,
                              const QModelIndex &index) const override;
};





#endif //DOUBLE_DELEGATE_H
