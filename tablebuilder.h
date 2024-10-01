#ifndef TABLEBUILDER_H
#define TABLEBUILDER_H

#include <QObject>
#include "lpmethods.h"

class TableBuilder : public QObject
{
    Q_OBJECT
public:
    explicit TableBuilder(LPMethod* method, QObject *parent = nullptr);

protected:
    LPMethod* currentMethod;

private:
    void MakeGeneralTable();

protected:
    void ConstructSimplexTable();

    void ConstructDualTable();

signals:
};

#endif // TABLEBUILDER_H
