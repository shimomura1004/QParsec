#include "if.h"

namespace qscheme {
namespace ast {

If::If(SharedVal c, SharedVal l, SharedVal r) : condition(c), left(l), right(r) {}

SharedVal If::create(SharedVal c, SharedVal l, SharedVal r) {
    return QSharedPointer<If>(new If(c, l, r));
}

QString If::toString() {
    return QStringLiteral("(if %1 %2 %3)").arg(condition->toString(), left->toString(), right->toString());
}

}
}
