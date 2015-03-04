#include "apply.h"
#include <QStringList>

namespace ast {

Apply::Apply(SharedVal p, QList<SharedVal> a) : proc(p), args(a) {}

SharedVal Apply::create(SharedVal p, QList<SharedVal> a) {
    return QSharedPointer<Apply>(new Apply(p, a));
}

QString Apply::toString() {
    QStringList result;
    for(const SharedVal& arg : args)
        result.push_back(arg->toString());

    return QStringLiteral("(%1 %2)").arg(proc->toString(), result.join(" "));
}

}
