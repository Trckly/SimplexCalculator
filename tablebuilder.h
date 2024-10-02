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

    QStringList headers = {"Base", "c_b", "Plan", "Ratio"};

    int tableWidth = 0;
    int tableLength = 0;

private:
    void ConstructTable();

protected:

signals:
};

#endif // TABLEBUILDER_H
