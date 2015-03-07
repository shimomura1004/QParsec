#ifndef QSCHEME_AST_CASE_H
#define QSCHEME_AST_CASE_H

#include "val.h"

namespace qscheme {
namespace ast {

class Case : public Val {
public:
    struct CaseClause {
        QList<SharedVal> data;
        QList<SharedVal> sequence;
        CaseClause(QList<SharedVal> d, QList<SharedVal> s);
        QString toString();
    };

protected:
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
}

#endif // QSCHEME_AST_CASE_H
