#include "cond.h"

namespace ast {

Cond::TestSeq::TestSeq(SharedVal t, QList<SharedVal> s) : test(t), sequences(s) {}
QString Cond::TestSeq::toString(){
    QStringList result;
    for(const auto& seq : sequences)
        result.push_back(seq->toString());
    return QStringLiteral("(%1 %2)").arg(test->toString(), result.join(" "));
}

Cond::Test::Test(SharedVal t) : test(t) {}
QString Cond::Test::toString(){
    return QStringLiteral("(%1)").arg(test->toString());
}

Cond::TestArrow::TestArrow(SharedVal t, SharedVal r) : test(t), recipient(r) {}
QString Cond::TestArrow::toString(){
    return QStringLiteral("(%1 => %2)").arg(test->toString(), recipient->toString());
}


Cond::Cond(QList<QSharedPointer<Cond::CondClause> > c) : condclauses(c) {}
Cond::Cond(QList<SharedVal> e) : elseclause(e) {}
Cond::Cond(QList<QSharedPointer<Cond::CondClause> > c, QList<SharedVal> e) : condclauses(c), elseclause(e) {}

SharedVal Cond::create(QList<QSharedPointer<Cond::CondClause> > c) { return QSharedPointer<Cond>(new Cond(c)); }
SharedVal Cond::create(QList<SharedVal> e) { return QSharedPointer<Cond>(new Cond(e)); }
SharedVal Cond::create(QList<QSharedPointer<Cond::CondClause> > c, QList<SharedVal> e) { return QSharedPointer<Cond>(new Cond(c, e)); }

QString Cond::toString() {
    QStringList conds;

    for(const auto& clause : condclauses)
        conds.push_back(clause->toString());

    if (elseclause.isEmpty())
        return QStringLiteral("(cond %1)").arg(conds.join(" "));

    QStringList elses;
    for(const auto& exp : elseclause)
        elses.push_back(exp->toString());

    return QStringLiteral("(cond %1 (else %2))").arg(conds.join(" "), elses.join(" "));
}

}
