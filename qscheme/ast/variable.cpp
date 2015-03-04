#include "variable.h"

namespace qscheme {
namespace ast {

Variable::Variable(QString s) : val(s) {}

SharedVal Variable::create(QString s) {
    return QSharedPointer<Variable>(new Variable(s));
}

QString Variable::toString() {
    return val;
}

}
}
