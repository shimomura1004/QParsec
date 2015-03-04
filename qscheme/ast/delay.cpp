#include "delay.h"

namespace qscheme {
namespace ast {

Delay::Delay(SharedVal v) : val(v) {}

SharedVal Delay::create(SharedVal v) {
    return QSharedPointer<Delay>(new Delay(v));
}

QString Delay::toString() {
    return QStringLiteral("(delay %1)").arg(val->toString());
}

}
}
