#ifndef LPMETHODS_H
#define LPMETHODS_H

#include <QObject>
#include <boost/multiprecision/cpp_dec_float.hpp>

using namespace boost::multiprecision;

struct LpStructure {
    QVector<cpp_dec_float_100> objFuncCoeffVector;
    QVector<QVector<cpp_dec_float_100>> constrCoeffMatrix;
    QVector<int> signs;
    QVector<cpp_dec_float_100> plans;
    int leadRowIndex;
    int leadColIndex;
    QVector<int> baseIndexes;
    cpp_dec_float_100 resultValue;
    QVector<cpp_dec_float_100> ratio;
    QVector<cpp_dec_float_100> lastRow;
    cpp_dec_float_100 leadElement;
};

class LPMethod : public QObject
{
    Q_OBJECT
public:
    explicit LPMethod(const QVector<cpp_dec_float_100>& objFuncCoeffVector, const QVector<QVector<cpp_dec_float_100>>& constrCoeffMatrix,
                       const QVector<int>& signs, const QVector<cpp_dec_float_100>& plans, QObject *parent = nullptr);

    LPMethod(const LpStructure& otherStructure);

    LPMethod();

protected:
    LpStructure structure;

    cpp_dec_float_100 leadingElement;

public:
    virtual bool SolveOneStep();

    const LpStructure &GetAll();

    virtual void InjectStructure(const LpStructure& otherStructure);

protected:
    bool SquareRule();

    virtual bool IsSolved() = 0;

    void ApplySignEffect();

    // To be called in child constructors because thay can change initial structure
    void GeneralSetup();

    void SafeInjectStructure(const LpStructure& otherStructure);

    virtual void RatioSetup() = 0;

    virtual void CalculateLeadingElement() = 0;

private:   
    void SetupBaseIndexes();
    void SetupLastRow();
    void SetupConstraintsCoefficientMatrix();

signals:
};

#endif // LPMETHODS_H
