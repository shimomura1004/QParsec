#ifndef CASE_H
#define CASE_H

#include "val.h"

namespace ast {

class Case : public Val {
protected:
    struct CaseClause {
        QList<SharedVal> data;
        QList<SharedVal> sequence;
        CaseClause(QList<SharedVal> d, QList<SharedVal> s);
        QString toString();
    };

    SharedVal caseexp;
    QList<QSharedPointer<CaseClause>> caseclauses;
    QList<SharedVal> elseclause;

    Case(SharedVal exp, QList<QSharedPointer<CaseClause>> c);
    Case(SharedVal exp, QList<SharedVal> e);
    Case(SharedVal exp, QList<QSharedPointer<CaseClause>> c, QList<SharedVal> e);

public:
    static SharedVal create(SharedVal exp, QList<QSharedPointer<CaseClause>> c);
    static SharedVal create(SharedVal exp, QList<SharedVal> e);
    static SharedVal create(SharedVal exp, QList<QSharedPointer<CaseClause>> c, QList<SharedVal> e);

    QString toString();
};

}

#endif // CASE_H
