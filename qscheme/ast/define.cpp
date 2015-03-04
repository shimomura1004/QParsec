#include "define.h"

namespace ast {

Define::Define(QString n, SharedVal b) : name(n), body(b) {}

SharedVal Define::create(QString n, SharedVal b) {
    return QSharedPointer<Define>(new Define(n, b));
}

QString Define::toString() {
    return QStringLiteral("(define %1 %2)").arg(name, body->toString());
}

}
