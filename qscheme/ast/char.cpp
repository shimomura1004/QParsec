#include "char.h"

namespace qscheme {
namespace ast {

Char::Char(QChar c) : val(c) {}

SharedVal Char::create(QChar c) {
    return QSharedPointer<Char>(new Char(c));
}

QString Char::toString() {
    return QStringLiteral("#\\%1").arg(val);
}

}
}
