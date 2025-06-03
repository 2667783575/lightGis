//
// Created by eric on 6/1/25.
//

#ifndef EXPR_FUNCTION_H
#define EXPR_FUNCTION_H
#include <vector>
#include <QVector>
#include "../external/exprtk.hpp"

class ExprFunction {
    using function_t = std::function<double(const std::vector<double>&)>;
public:
    ExprFunction(const QString & expression,const QVector<QString>& variables);
    ExprFunction(const ExprFunction& expr) {
        this->function = expr.function;

    };
    double operator()(const std::vector<double>& values);
    bool changeFunction(const QString & expression,const QVector<QString>& variables);
private:
    std::vector<double> var_values;
    std::vector<double *> var_ptrs;
    exprtk::parser<double> parser;
    function_t function;
};



#endif //EXPR_FUNCTION_H
