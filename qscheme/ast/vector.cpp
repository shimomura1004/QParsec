#include "vector.h"
#include <QStringList>

namespace qscheme {
namespace ast {

Vector::Vector(QList<SharedVal> e) : elems(e) {}

SharedVal Vector::create(QList<SharedVal> e) {
    return QSharedPointer<Vector>(new Vector(e));
}

QString Vector::toString() {
    QStringList result;
    for(const SharedVal& e : elems)
        result.push_back(e->toString());

    return QStringLiteral("#(%1)").arg(result.join(" "));
}

}
}
