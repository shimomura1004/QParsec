#include "begin.h"
#include <QStringList>

namespace qscheme {
namespace ast {

Begin::Begin(QList<SharedVal> v) : vals(v) {}

SharedVal Begin::create(QList<SharedVal> v) {
    return QSharedPointer<Begin>(new Begin(v));
}

QString Begin::toString() {
    QStringList result;
    for(const auto& v : vals)
        result.push_back(v->toString());

    return QStringLiteral("(begin %1)").arg(result.join(" "));
}

}
}
