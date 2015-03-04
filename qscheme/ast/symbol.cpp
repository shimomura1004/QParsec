#include "symbol.h"

namespace qscheme {
namespace ast {

Symbol::Symbol(QString s) : val(s) {}

SharedVal Symbol::create(QString s) {
    return QSharedPointer<Symbol>(new Symbol(s));
}

QString Symbol::toString() {
    return val;
}

}
}
