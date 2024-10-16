#ifndef SIMPLEXTABLEBUILDER_H
#define SIMPLEXTABLEBUILDER_H

#include <QObject>
#include <QTableWidget>
#include "lpmethods.h"
#include <boost/multiprecision/cpp_dec_float.hpp>

using namespace boost::multiprecision;

class SimplexTableBuilder : public QObject
{
    Q_OBJECT
public:
    explicit SimplexTableBuilder(LPMethod* method, QObject *parent = nullptr);

protected:
    LPMethod* currentMethod;

    QStringList initHeaders = {"Base", "c_b", "Plan"};
    QStringList currentHeaders;

    int tableWidth = 0;
    int tableLength = 0;

public:
    QTableWidget* ConstructTable();

    void MarkLeadingElement(QTableWidget* tableToMark);

private:
    void AppendRatio(QTableWidget* table, const LpStructure& structure);
};

#endif // SIMPLEXTABLEBUILDER_H
