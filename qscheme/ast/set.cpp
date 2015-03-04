#include "set.h"

namespace qscheme {
namespace ast {

Set::Set(QString v, SharedVal e) : var(v), exp(e) {}

SharedVal Set::create(QString v, SharedVal e) {
    return QSharedPointer<Set>(new Set(v, e));
}

QString Set::toString() {
    return QStringLiteral("(set! %1 %2)").arg(var, exp->toString());
}

}
}
