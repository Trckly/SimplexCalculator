#ifndef SIMPLEXCLASS_H
#define SIMPLEXCLASS_H

#include <QObject>
#include "lpmethods.h"

class SimplexClass : public LPMethod
{
    Q_OBJECT
public:
    explicit SimplexClass(const QVector<float>& objFuncCoeffVector, const QVector<QVector<float>>& constrCoeffMatrix,
                             const QVector<int>& signs, const QVector<float>& plans, QObject *parent = nullptr);

public:

private:
    int GetMinColumnIndex();

    int GetMinRowIndex(int colIndex);

    virtual void CalculateLeadingElement() override;

signals:
};

#endif // SIMPLEXCLASS_H
