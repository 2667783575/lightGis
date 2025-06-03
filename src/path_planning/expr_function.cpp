//
// Created by eric on 6/1/25.
//

#include "expr_function.h"

#include <QMessageBox>

ExprFunction::ExprFunction(const QString &expression, const QVector<QString> &variables) {
    var_values.resize(variables.size(), 0.0);
    var_ptrs.resize(variables.size(), nullptr);
    exprtk::expression<double> expr;
    exprtk::symbol_table<double> symbol_table;
    for (auto i = 0; i < variables.size(); i++) {
        symbol_table.add_variable(variables[i].toStdString(), var_values[i]);
        var_ptrs.push_back(&var_values[i]);
    }
    expr.register_symbol_table(symbol_table);
    if (!parser.compile(expression.toStdString(),expr)) {
        QMessageBox::warning(nullptr,"sorry","Something go wrong");
    }
    function = [this,expr](const std::vector<double>& values)mutable->double {
        for (auto i = 0 ;i < values.size(); i++) {
            var_values[i] = values[i];
        }
        return expr.value();
    };
}

bool ExprFunction::changeFunction(const QString &expression, const QVector<QString> &variables) {
    var_values.resize(variables.size(), 0.0);
    var_ptrs.resize(variables.size(), nullptr);
    exprtk::symbol_table<double> symbol_table;
    exprtk::expression<double> expr;
    for (auto i = 0; i < variables.size(); i++) {
        symbol_table.add_variable(variables[i].toStdString(), var_values[i]);
        var_ptrs.push_back(&var_values[i]);
    }
    expr.register_symbol_table(symbol_table);
    if (!parser.compile(expression.toStdString(),expr)) {
        QMessageBox::warning(nullptr,"sorry","Something go wrong");
        return false;
    }
    function = [this,expr](const std::vector<double>& values)mutable->double {
        for (auto i = 0 ;i < values.size(); i++) {
            var_values[i] = values[i];
        }
        return expr.value();
    };
    return true;
}

double ExprFunction::operator()(const std::vector<double> &values) {
    return function(values);
}
