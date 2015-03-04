#ifndef QSCHEME_AST_VAL_H
#define QSCHEME_AST_VAL_H

#include <QString>
#include <QSharedPointer>

namespace qscheme {
namespace ast {

class Val {
public:
    virtual ~Val(){}
    virtual QString toString() = 0;
};

typedef QSharedPointer<Val> SharedVal;
typedef QList<QPair<QString, SharedVal>> Env;

}
}

#endif // VAL_QSCHEME_AST_H

