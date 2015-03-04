#include "integer.h"

namespace ast {

Integer::Integer(int i) : val(i) {}

SharedVal Integer::create(int64_t i) {
    return QSharedPointer<Integer>(new Integer(i));
}

QString Integer::toString() {
    return QString::number(val);
}

}
