#include "case.h"
#include <QStringList>

namespace ast {

Case::CaseClause::CaseClause(QList<SharedVal> d, QList<SharedVal> s) : data(d), sequence(s) {}

QString Case::CaseClause::toString() {
    QStringList datastr;
    for(const auto& datum : data)
        datastr.push_back(datum->toString());

    QStringList seqstr;
    for(const auto& seq : sequence)
        seqstr.push_back(seq->toString());

    return QStringLiteral("((%1) %2)").arg(datastr.join(" "), seqstr.join(" "));
}


Case::Case(SharedVal exp, QList<QSharedPointer<Case::CaseClause> > c) : caseexp(exp), caseclauses(c) {}
Case::Case(SharedVal exp, QList<SharedVal> e) : caseexp(exp), elseclause(e) {}
Case::Case(SharedVal exp, QList<QSharedPointer<Case::CaseClause> > c, QList<SharedVal> e) : caseexp(exp), caseclauses(c), elseclause(e) {}

SharedVal Case::create(SharedVal exp, QList<QSharedPointer<Case::CaseClause> > c)  { return QSharedPointer<Case>(new Case(exp, c)); }
SharedVal Case::create(SharedVal exp, QList<SharedVal> e) { return QSharedPointer<Case>(new Case(exp, e)); }
SharedVal Case::create(SharedVal exp, QList<QSharedPointer<Case::CaseClause> > c, QList<SharedVal> e) { return QSharedPointer<Case>(new Case(exp, c, e)); }

QString Case::toString() {
    QStringList cases;
    for(const auto &c : caseclauses)
        cases.push_back(c->toString());

    if (elseclause.isEmpty())
        return QStringLiteral("(case %1 %2)").arg(caseexp->toString(), cases.join(" "));

    QStringList elses;
    for(const auto& exp : elseclause)
        elses.push_back(exp->toString());

    return QStringLiteral("(case %1 %2 (else %3))").arg(caseexp->toString(), cases.join(" "), elses.join(" "));
}

}
