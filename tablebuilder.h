#ifndef TABLEBUILDER_H
#define TABLEBUILDER_H

#include <QObject>

class TableBuilder : public QObject
{
    Q_OBJECT
public:
    explicit TableBuilder(QObject *parent = nullptr);

private:

protected:
    void ConstructTable();

signals:
};

#endif // TABLEBUILDER_H
