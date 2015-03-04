#ifndef LAMBDA_H
#define LAMBDA_H

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

#endif // LAMBDA_H
