#include "or.h"
#include <QStringList>

namespace ast {

Or::Or(QList<SharedVal> vs) : vals(vs) {}

SharedVal Or::create(QList<SharedVal> vs) {
    return QSharedPointer<Or>(new Or(vs));
}

QString Or::toString() {
    QStringList result;
    for(const auto& v : vals)
        result.push_back(v->toString());

    return QStringLiteral("(or %1)").arg(result.join(" "));
}

}
