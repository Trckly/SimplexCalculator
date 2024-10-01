#ifndef NEWSIMPLEXCLASS_H
#define NEWSIMPLEXCLASS_H

#include <QObject>
#include "lpmethods.h"

class NewSimplexClass : public LPMethods
{
    Q_OBJECT
public:
    explicit NewSimplexClass(QObject *parent = nullptr);

public:
    virtual bool SolveOneStep(QVector<float>& objFuncCoeffVector, QVector<QVector<float>>& constrCoeffMatrix,
                              QVector<int>& signs, QVector<float>& plans, int& outLeadRowIndex, int& outLeadColIndex,
                              QVector<int>& outBaseIndexes, float& outResultValue, QVector<float>& outRatio,
                              QVector<float>& outLastRow) override;


private:
    int GetMinColumnIndex();

    int GetMinRowIndex(int colIndex);

signals:
};

#endif // NEWSIMPLEXCLASS_H
