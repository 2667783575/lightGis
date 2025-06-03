#ifndef PROPERTY_EDITOR_H
#define PROPERTY_EDITOR_H

#include "double_delegate.h"
#include <QDialog>
#include <QTableWidget>
#include <QPushButton>
#include <QMap>
#include <vector>
#include <map>

class PropertyEditor : public QDialog {
    Q_OBJECT

public:
    explicit PropertyEditor(QWidget *parent = nullptr);

    void setValue(long long nodeId, int propertyIndex, double value);
    void updateDistances(const std::vector<std::pair<long long, double>>& distances);

    QStringList properties() const { return properties_; }
    std::map<long long, std::vector<double>> data() const { return data_; }

    signals:
        void updated();

private slots:
    void addProperty();
    void removeProperty();
    void updateValue(int row, int column);
    void importCSV();
    void exportToCSV();

private:
    void createTable();
    void addFixedProperties();
    void refreshTable();
    int findColumnForProperty(const QString& propertyName);
    void updateNodeRow(long long nodeId, int row);

    QWidget *button_widget_;
    QWidget *property_manage_widget_;
    QTableWidget *table_;
    QPushButton *add_property_btn_;
    QPushButton *remove_property_btn_;
    QPushButton *import_btn_;
    QPushButton *export_btn_;

    QStringList properties_; // 属性名称列表
    std::map<long long, std::vector<double>> data_; // 数据存储 [节点ID][属性]
    std::vector<long long> sortedKeys_; // 排序后的节点ID
};

#endif