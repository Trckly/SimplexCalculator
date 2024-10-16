#ifndef SIMPLEXCLASS_H
#define SIMPLEXCLASS_H

#include <QObject>
#include "lpmethods.h"


class SimplexClass : public LPMethod
{
    Q_OBJECT
public:
    explicit SimplexClass(const QVector<cpp_dec_float_100>& objFuncCoeffVector, const QVector<QVector<cpp_dec_float_100>>& constrCoeffMatrix,
                             const QVector<int>& signs, const QVector<cpp_dec_float_100>& plans, QObject *parent = nullptr);
    SimplexClass(const LpStructure& otherStructure);
    SimplexClass();

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
