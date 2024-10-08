#ifndef TABLEBUILDER_H
#define TABLEBUILDER_H

#include <QObject>
#include <QTableWidget>
#include "lpmethods.h"

class TableBuilder : public QObject
{
    Q_OBJECT
public:
    explicit TableBuilder(LPMethod* method, QObject *parent = nullptr);

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

#endif // TABLEBUILDER_H
