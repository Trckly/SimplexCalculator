#ifndef NEWSIMPLEXCLASS_H
#define NEWSIMPLEXCLASS_H

#include <QObject>
#include "lpmethods.h"

class NewSimplexClass : public LPMethod
{
    Q_OBJECT
public:
    explicit NewSimplexClass(const QVector<float>& objFuncCoeffVector, const QVector<QVector<float>>& constrCoeffMatrix,
                             const QVector<int>& signs, const QVector<float>& plans, QObject *parent = nullptr);

public:
    virtual bool SolveOneStep() override;


private:
    int GetMinColumnIndex();

    int GetMinRowIndex(int colIndex);

    virtual void ApplySignEffect() override;

signals:
};

#endif // NEWSIMPLEXCLASS_H
