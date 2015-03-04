#ifndef COND_H
#define COND_H

#include <QStringList>
#include "val.h"

namespace qscheme {
namespace ast {

class Cond : public Val {
protected:
    struct CondClause {
        virtual QString toString() = 0;
        virtual ~CondClause() {}
    };
    struct TestSeq : CondClause {
        SharedVal test;
        QList<SharedVal> sequences;
        TestSeq(SharedVal t, QList<SharedVal> s);
        QString toString();
    };
    struct Test : CondClause {
        SharedVal test;
        Test(SharedVal t);
        QString toString();
    };
    struct TestArrow : CondClause {
        SharedVal test;
        SharedVal recipient;
        TestArrow(SharedVal t, SharedVal r);
        QString toString();
    };

    QList<QSharedPointer<CondClause>> condclauses;
    QList<SharedVal> elseclause;

    Cond(QList<QSharedPointer<CondClause>> c);
    Cond(QList<SharedVal> e);
    Cond(QList<QSharedPointer<CondClause>> c, QList<SharedVal> e);

public:
    static SharedVal create(QList<QSharedPointer<CondClause>> c);
    static SharedVal create(QList<SharedVal> e);
    static SharedVal create(QList<QSharedPointer<CondClause>> c, QList<SharedVal> e);

    QString toString();
};

}
}

#endif // COND_H
