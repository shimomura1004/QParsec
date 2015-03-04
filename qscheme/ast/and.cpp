#include "and.h"
#include <QStringList>

namespace qscheme {
namespace ast {

And::And(QList<SharedVal> vs) : vals(vs) {}

SharedVal And::create(QList<SharedVal> vs) {
    return QSharedPointer<And>(new And(vs));
}

QString And::toString() {
    QStringList result;
    for(const auto& v : vals)
        result.push_back(v->toString());

    return QStringLiteral("(and %1)").arg(result.join(" "));
}

}
}
