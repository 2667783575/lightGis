//
// Created by eric on 5/31/25.
//

#include "double_delegate.h"


QWidget *DoubleDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &option,
                      const QModelIndex &index) const {
    QDoubleSpinBox *editor = new QDoubleSpinBox(parent);
    editor->setRange(-999999.0, 999999.0);
    editor->setDecimals(4);
    return editor;
}

void DoubleDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const {
    double value = index.data(Qt::EditRole).toDouble();
    QDoubleSpinBox *spinBox = static_cast<QDoubleSpinBox*>(editor);
    spinBox->setValue(value);
}

void DoubleDelegate::setModelData(QWidget *editor, QAbstractItemModel *model,
                  const QModelIndex &index) const  {
    QDoubleSpinBox *spinBox = static_cast<QDoubleSpinBox*>(editor);
    spinBox->interpretText();
    double value = spinBox->value();
    model->setData(index, QString::number(value, 'f', 2), Qt::EditRole);
}

void DoubleDelegate::updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option,
                          const QModelIndex &index) const  {
    editor->setGeometry(option.rect);
}