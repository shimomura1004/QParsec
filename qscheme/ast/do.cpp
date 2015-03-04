#include "do.h"
#include <QStringList>

namespace ast {

QSharedPointer<Do::IterationSpec> Do::IterationSpec::create(QString v, SharedVal i) {
    return QSharedPointer<IterationSpec>(new IterationSpec(v, i));
}
Do::IterationSpec::IterationSpec(QString v, SharedVal i) : var(v), init(i) {}
Do::IterationSpec::~IterationSpec() {}
QString Do::IterationSpec::toString() {
    return QStringLiteral("(%1 %2)").arg(var, init->toString());
}

QSharedPointer<Do::IterationSpecWithStep> Do::IterationSpecWithStep::create(QString v, SharedVal i, SharedVal s) {
    return QSharedPointer<IterationSpecWithStep>(new IterationSpecWithStep(v, i, s));
}
Do::IterationSpecWithStep::IterationSpecWithStep(QString v, SharedVal i, SharedVal s) : IterationSpec(v, i), step(s) {}
Do::IterationSpecWithStep::~IterationSpecWithStep() {}
QString Do::IterationSpecWithStep::toString() {
    return QStringLiteral("(%1 %2 %3)").arg(var, init->toString(), step->toString());
}

Do::Do(QList<QSharedPointer<Do::IterationSpec> > i, SharedVal t, SharedVal d, QList<SharedVal> c) : iterationspecs(i), test(t), doresult(d), commands(c) {}

SharedVal Do::create(QList<QSharedPointer<Do::IterationSpec> > i, SharedVal t, SharedVal d, QList<SharedVal> c) {
    return QSharedPointer<Do>(new Do(i, t, d, c));
}

QString Do::toString() {
    QStringList is;
    for(const auto& i : iterationspecs)
        is.push_back(i->toString());

    if (commands.isEmpty()) {
        return QStringLiteral("(do (%1) (%2 %3))").arg(is.join(" "), test->toString(), doresult->toString());
    }

    QStringList cs;
    for(const auto& c : commands)
        cs.push_back(c->toString());

    return QStringLiteral("(do (%1) (%2 %3) %4)").arg(is.join(" "), test->toString(), doresult->toString(), cs.join(" "));
}

}
