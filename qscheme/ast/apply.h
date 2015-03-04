#ifndef QSCHEME_AST_APPLY_H
#define QSCHEME_AST_APPLY_H

#include "val.h"

namespace qscheme {
namespace ast {

class Apply : public Val {
protected:
    SharedVal proc;
    QList<SharedVal> args;
    Apply(SharedVal p, QList<SharedVal> a);

public:
    static SharedVal create(SharedVal p, QList<SharedVal> a);
    QString toString();
};

}
}

#endif // QSCHEME_AST_APPLY_H
