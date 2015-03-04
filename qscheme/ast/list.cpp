#include "list.h"
#include <QStringList>

namespace ast {

List::List(QList<SharedVal> l) : val(l) {}

SharedVal List::create(QList<SharedVal> l) {
    return QSharedPointer<List>(new List(l));
}

QString List::toString() {
    QStringList result;
    for(const SharedVal& v : val)
        result.push_back(v->toString());

    return QStringLiteral("(%1)").arg(result.join(" "));
}

}
