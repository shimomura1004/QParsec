#include "real.h"

namespace qscheme {
namespace ast {

Real::Real(double d) : val(d) {}

SharedVal Real::create(double d) {
    return QSharedPointer<Real>(new Real(d));
}

QString Real::toString() {
    return QStringLiteral("%1").arg(val, 0, 'e');
}

}
}
