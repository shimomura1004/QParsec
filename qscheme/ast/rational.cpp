#include "rational.h"

namespace ast {

Rational::Rational(int64_t n, uint64_t d) : numerator(n), denominator(d) {}

SharedVal Rational::create(int64_t n, uint64_t d) {
    return QSharedPointer<Rational>(new Rational(n, d));
}

QString Rational::toString() {
    return QStringLiteral("%1/%2").arg(numerator).arg(denominator);
}

}
