#include "complex.h"

namespace qscheme {
namespace ast {

Complex::Complex(double r, double i) : real(r), imagnary(i) {}

SharedVal Complex::create(double r, double i) {
    return QSharedPointer<Complex>(new Complex(r, i));
}

QString Complex::toString() {
    if (imagnary > 0.0)
        return QStringLiteral("%1+%2i").arg(real).arg(imagnary);
    return QStringLiteral("%1%2i").arg(real).arg(imagnary);
}

}
}
