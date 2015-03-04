#include "bool.h"

namespace qscheme {
namespace ast {

Bool::Bool(bool b) : val(b) {}

SharedVal Bool::create(bool b) {
    return QSharedPointer<Bool>(new Bool(b));
}

QString Bool::toString() {
    return QStringLiteral("#%1").arg(val ? "t" : "f");
}

}
}
