#include "lambda.h"

namespace ast {

Lambda::Lambda(QList<QString> a, QString l, QList<SharedVal> b, Env e) : args(a), listarg(l), bodies(b), env(e) {}

SharedVal Lambda::create(QList<QString> a, QString l, QList<SharedVal> b, Env e) {
    return QSharedPointer<Lambda>(new Lambda(a, l, b, e));
}

QString Lambda::toString() {
    QStringList bs;
    for(const auto& b : bodies)
        bs.push_back(b->toString());

    if (listarg.isEmpty())
        return QStringLiteral("(lambda (%1) %2)").arg(args.join(" "), bs.join(" "));

    if (args.isEmpty())
        return QStringLiteral("(lambda %1 %2)").arg(listarg, bs.join(" "));

    return QStringLiteral("(lambda (%1 . %2)  %3)").arg(args.join(" "), listarg, bs.join(" "));
}

}
