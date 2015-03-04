#ifndef APPLY_H
#define APPLY_H

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

#endif // APPLY_H
