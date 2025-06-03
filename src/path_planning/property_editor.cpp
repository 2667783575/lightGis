#include "property_editor.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QHeaderView>
#include <QInputDialog>
#include <QMessageBox>
#include <QFileDialog>
#include <QTextStream>
#include <algorithm>
#include <QDebug>

PropertyEditor::PropertyEditor(QWidget *parent)
    : QDialog(parent) {
    // 创建主布局
    auto *mainLayout = new QVBoxLayout(this);
    
    // 创建属性管理区域
    property_manage_widget_ = new QWidget(this);
    auto *propertyLayout = new QHBoxLayout(property_manage_widget_);
    auto *propertyLabel = new QLabel("属性管理:");
    add_property_btn_ = new QPushButton("添加属性");
    remove_property_btn_ = new QPushButton("删除属性");

    propertyLayout->addWidget(propertyLabel);
    propertyLayout->addWidget(add_property_btn_);
    propertyLayout->addWidget(remove_property_btn_);
    propertyLayout->addStretch();
    
    // 创建表格
    createTable();

    // 创建按钮区域
    button_widget_ = new QWidget(this);
    auto *buttonLayout = new QHBoxLayout(button_widget_);
    import_btn_ = new QPushButton("导入CSV");
    export_btn_ = new QPushButton("导出CSV");
    buttonLayout->addWidget(import_btn_);
    buttonLayout->addWidget(export_btn_);

    // 添加到主布局
    mainLayout->addWidget(property_manage_widget_);
    mainLayout->addWidget(table_);
    mainLayout->addWidget(button_widget_);

    // 初始化属性
    addFixedProperties();

    // 连接信号槽
    connect(add_property_btn_, &QPushButton::clicked, this, &PropertyEditor::addProperty);
    connect(remove_property_btn_, &QPushButton::clicked, this, &PropertyEditor::removeProperty);
    connect(import_btn_, &QPushButton::clicked, this, &PropertyEditor::importCSV);
    connect(export_btn_, &QPushButton::clicked, this, &PropertyEditor::exportToCSV);
    connect(table_, &QTableWidget::cellChanged, this, &PropertyEditor::updateValue);
}

void PropertyEditor::createTable() {
    table_ = new QTableWidget(0, 1, this); // 初始0行1列(距离)
    table_->setHorizontalHeaderLabels({"distance"}); // 第一个固定属性
    table_->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    table_->verticalHeader()->setSectionResizeMode(QHeaderView::Fixed);
    table_->verticalHeader()->setDefaultSectionSize(30);

    // 设置委托，使所有单元格只能输入double
    table_->setItemDelegate(new DoubleDelegate(this));
}

void PropertyEditor::addFixedProperties() {
    properties_ << "distance"; // 固定属性
}

void PropertyEditor::refreshTable() {
    // 断开信号防止递归更新
    table_->blockSignals(true);
    
    // 清空表格
    table_->setRowCount(0);
    
    // 获取排序后的节点ID
    sortedKeys_.clear();
    for (const auto& pair : data_) {
        sortedKeys_.push_back(pair.first);
    }
    std::sort(sortedKeys_.begin(), sortedKeys_.end());
    
    // 设置行数
    table_->setRowCount(sortedKeys_.size());
    
    // 填充表格
    for (int row = 0; row < sortedKeys_.size(); ++row) {
        long long nodeId = sortedKeys_[row];
        updateNodeRow(nodeId, row);
    }
    
    // 重新连接信号
    table_->blockSignals(false);
}

void PropertyEditor::updateNodeRow(long long nodeId, int row) {
    // 设置行表头
    table_->setVerticalHeaderItem(row, new QTableWidgetItem(QString::number(nodeId)));
    
    // 确保属性向量大小正确
    std::vector<double>& values = data_[nodeId];
    if (values.size() < static_cast<size_t>(properties_.size())) {
        values.resize(properties_.size(), 0.0);
    }
    
    // 填充单元格
    for (int col = 0; col < properties_.size(); ++col) {
        if (col >= table_->columnCount()) {
            table_->insertColumn(col);
            table_->setHorizontalHeaderItem(col, new QTableWidgetItem(properties_[col]));
        }
        
        QTableWidgetItem *item = new QTableWidgetItem(QString::number(values[col], 'f', 5));
        table_->setItem(row, col, item);
    }
}

int PropertyEditor::findColumnForProperty(const QString& propertyName) {
    for (int col = 0; col < table_->columnCount(); ++col) {
        if (table_->horizontalHeaderItem(col)->text() == propertyName) {
            return col;
        }
    }
    return -1;
}

void PropertyEditor::updateDistances(const std::vector<std::pair<long long, double>>& distances) {
    // 更新数据模型
    for (const auto& pair : distances) {
        long long nodeId = pair.first;
        double distance = pair.second;
        
        // 如果节点不存在，创建新条目
        if (data_.find(nodeId) == data_.end()) {
            data_[nodeId] = std::vector<double>(properties_.size(), 0.0);
        }
        
        // 更新距离值（属性索引0）
        data_[nodeId][0] = distance;
    }
    
    // 刷新表格
    refreshTable();
    emit updated();
}

void PropertyEditor::addProperty() {
    // 获取新属性名称
    bool ok;
    QString newProp = QInputDialog::getText(
        this, "添加属性", "请输入属性名称:",
        QLineEdit::Normal, QString("属性%1").arg(properties_.size()), &ok
    );

    if (!ok || newProp.isEmpty()) return;

    // 添加到属性列表
    properties_ << newProp;
    
    // 在数据模型中为所有节点添加新属性
    for (auto& pair : data_) {
        pair.second.push_back(0.0);
    }
    
    // 刷新表格
    refreshTable();
}

void PropertyEditor::removeProperty() {
    // 获取当前选中的列
    int col = table_->currentColumn();
    if (col == -1) {
        QMessageBox::information(this, "提示", "请选择要删除的属性列");
        return;
    }

    // 检查是否是固定属性"距离"
    if (col == 0) {
        QMessageBox::warning(this, "删除失败", "无法删除固定属性'距离'");
        return;
    }

    // 确认删除
    QString propName = table_->horizontalHeaderItem(col)->text();
    int ret = QMessageBox::question(
        this, "确认删除", QString("确定要删除属性 '%1' 吗?").arg(propName),
        QMessageBox::Yes | QMessageBox::No
    );

    if (ret != QMessageBox::Yes) return;

    // 从数据结构中移除
    properties_.removeAt(col);
    for (auto& pair : data_) {
        if (pair.second.size() > static_cast<size_t>(col)) {
            pair.second.erase(pair.second.begin() + col);
        }
    }

    // 刷新表格
    refreshTable();
}

void PropertyEditor::updateValue(int row, int column) {
    if (row < 0 || row >= sortedKeys_.size() || column < 0 || column >= properties_.size()) 
        return;

    long long nodeId = sortedKeys_[row];
    QTableWidgetItem *item = table_->item(row, column);
    if (!item) return;

    bool ok;
    double value = item->text().toDouble(&ok);
    if (ok) {
        // 更新数据模型
        if (data_[nodeId].size() <= static_cast<size_t>(column)) {
            data_[nodeId].resize(column + 1, 0.0);
        }
        data_[nodeId][column] = value;
        
        // 更新显示格式
        item->setText(QString::number(value, 'f', 5));
    }
}

void PropertyEditor::setValue(long long nodeId, int propertyIndex, double value) {
    if (data_.find(nodeId) == data_.end()) {
        // 如果节点不存在，创建新条目
        data_[nodeId] = std::vector<double>(properties_.size(), 0.0);
        sortedKeys_.push_back(nodeId);
        std::sort(sortedKeys_.begin(), sortedKeys_.end());
        refreshTable();
    }

    // 更新数据模型
    if (propertyIndex >= 0 && propertyIndex < properties_.size()) {
        if (data_[nodeId].size() <= static_cast<size_t>(propertyIndex)) {
            data_[nodeId].resize(propertyIndex + 1, 0.0);
        }
        data_[nodeId][propertyIndex] = value;
        
        // 查找节点所在行
        auto it = std::find(sortedKeys_.begin(), sortedKeys_.end(), nodeId);
        if (it != sortedKeys_.end()) {
            int row = std::distance(sortedKeys_.begin(), it);
            
            // 更新表格
            if (row < table_->rowCount() && propertyIndex < table_->columnCount()) {
                table_->blockSignals(true);
                QTableWidgetItem *item = table_->item(row, propertyIndex);
                if (item) {
                    item->setText(QString::number(value, 'f', 5));
                }
                table_->blockSignals(false);
            }
        }
    }
}

void PropertyEditor::importCSV() {
    QString fileName = QFileDialog::getOpenFileName(
        this, "导入CSV文件", "", "CSV Files (*.csv)"
    );

    if (fileName.isEmpty()) return;

    QFile file(fileName);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QMessageBox::warning(this, "导入错误", "无法打开文件");
        return;
    }

    QTextStream in(&file);
    QStringList lines;
    while (!in.atEnd()) {
        lines << in.readLine();
    }
    file.close();

    if (lines.size() < 2) {
        QMessageBox::warning(this, "导入错误", "文件内容不足");
        return;
    }

    // 解析表头
    QStringList headers = lines[0].split(',');
    if (headers.size() < 2) {
        QMessageBox::warning(this, "导入错误", "无效的表头格式");
        return;
    }

    // 移除表头中的"节点"列（如果有）
    if (headers[0].trimmed().toLower() == "节点") {
        headers.removeFirst();
    }

    // 检查距离属性是否存在
    bool hasDistance = false;
    for (const QString& header : headers) {
        if (header.trimmed() == "distance") {
            hasDistance = true;
            break;
        }
    }

    if (!hasDistance) {
        QMessageBox::warning(this, "导入错误", "CSV文件中缺少'距离'属性");
        return;
    }

    // 清空现有数据
    data_.clear();
    properties_ = headers;
    
    // 解析数据行
    for (int i = 1; i < lines.size(); i++) {
        QStringList values = lines[i].split(',');
        if (values.size() < headers.size() + 1) continue;

        // 第一列是节点ID
        bool ok;
        long long nodeId = values[0].trimmed().toLongLong(&ok);
        if (!ok) continue;

        // 创建节点数据
        std::vector<double> nodeData;
        for (int j = 1; j < values.size() && j <= headers.size(); j++) {
            nodeData.push_back(values[j].trimmed().toDouble());
        }
        
        // 确保数据大小与属性数量一致
        if (nodeData.size() < static_cast<size_t>(headers.size())) {
            nodeData.resize(headers.size(), 0.0);
        }
        
        data_[nodeId] = nodeData;
    }

    // 刷新表格
    refreshTable();
    QMessageBox::information(this, "导入成功", "CSV数据已成功导入");
}

void PropertyEditor::exportToCSV() {
    QString fileName = QFileDialog::getSaveFileName(
        this, "导出CSV", "", "CSV Files (*.csv)"
    );

    if (fileName.isEmpty()) return;

    QFile file(fileName);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QMessageBox::warning(this, "导出错误", "无法打开文件进行写入");
        return;
    }

    QTextStream stream(&file);

    // 写入表头
    stream << "节点";
    for (const QString& prop : properties_) {
        stream << "," << prop;
    }
    stream << "\n";

    // 写入数据
    for (long long nodeId : sortedKeys_) {
        stream << nodeId;
        const std::vector<double>& values = data_[nodeId];
        for (size_t i = 0; i < values.size(); i++) {
            stream << "," << QString::number(values[i], 'f', 5);
        }
        stream << "\n";
    }

    file.close();
    QMessageBox::information(this, "导出成功", "数据已成功导出为CSV文件");
}