#include "dlist.h"
#include <QStringList>

namespace qscheme {
namespace ast {

DList::DList(QList<SharedVal> a, SharedVal d) : cars(a), cdr(d) {}

SharedVal DList::create(QList<SharedVal> l, SharedVal d) {
    return QSharedPointer<DList>(new DList(l, d));
}

QString DList::toString() {
    QStringList result;
    for(const SharedVal& car : cars)
        result.push_back(car->toString());

    return QStringLiteral("(%1 . %2)").arg(result.join(" "), cdr->toString());
}

}
}
