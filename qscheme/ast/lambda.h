#ifndef QSCHEME_AST_LAMBDA_H
#define QSCHEME_AST_LAMBDA_H

#include <QStringList>
#include "val.h"

namespace qscheme {
namespace ast {

class Lambda : public Val {
protected:
    QStringList args;
    QString listarg;
    QList<SharedVal> bodies;
    Env env;
    Lambda(QList<QString> a, QString l, QList<SharedVal> b, Env e);

public:
    static SharedVal create(QList<QString> a, QString l, QList<SharedVal> b, Env e);
    QString toString();
};

}
}

#endif // QSCHEME_AST_LAMBDA_H
