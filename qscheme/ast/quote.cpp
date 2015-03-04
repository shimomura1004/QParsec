#include "quote.h"

namespace ast {

Quote::Quote(SharedVal v) : val(v) {}

SharedVal Quote::create(SharedVal v) {
    return QSharedPointer<Quote>(new Quote(v));
}

QString Quote::toString() {
    return QStringLiteral("%1").arg(val->toString());
}

}
