#include "let.h"
#include <QStringList>

namespace ast {

const char *Let::typeToString(Let::Type type) {
    switch(type) {
    case Normal: return "";
    case Star: return "*";
    case Rec: return "rec";
    }
}

Let::Let(QString n, QList<QPair<QString, SharedVal> > b, QList<SharedVal> s, Let::Type t) : name(n), bindings(b), sequence(s), type(t) {}

SharedVal Let::create(QString n, QList<QPair<QString, SharedVal> > b, QList<SharedVal> s, Let::Type t) {
    return QSharedPointer<Let>(new Let(n, b, s, t));
}

QString Let::toString() {
    QStringList bindingstr;
    for(const auto& b : bindings)
        bindingstr.push_back(QStringLiteral("(%1 %2)").arg(b.first, b.second->toString()));

    QStringList sequencestr;
    for(const auto &s : sequence)
        sequencestr.push_back(s->toString());

    if (name.isNull())
        return QStringLiteral("(let%1 (%2) %3)").arg(typeToString(type), bindingstr.join(" "), sequencestr.join(" "));

    return QStringLiteral("(let%1 %2 (%3) %4)").arg(typeToString(type), name, bindingstr.join(" "), sequencestr.join(" "));
}

}
