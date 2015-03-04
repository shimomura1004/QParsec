#include "undef.h"

namespace ast {

SharedVal Undef::create() {
    return QSharedPointer<Undef>(new Undef());
}

QString Undef::toString() {
    return QStringLiteral("#<undef>");
}

}
