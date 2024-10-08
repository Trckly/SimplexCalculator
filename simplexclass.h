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
    SimplexClass(const LpStructure& otherStructure);

public:

private:
    void InitializeClass();

    int GetMinColumnIndex();

    int GetMinRowIndex(int colIndex);

protected:
    virtual void CalculateLeadingElement() override;
    virtual void RatioSetup() override;
    virtual bool IsSolved() override;

};

#endif // SIMPLEXCLASS_H
