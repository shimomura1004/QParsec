#ifndef QSCHEME_AST_OR_H
#define QSCHEME_AST_OR_H

#include "val.h"

namespace qscheme {
namespace ast {

class Or : public Val {
protected:
    QList<SharedVal> vals;
    Or(QList<SharedVal> vs);

public:
    static SharedVal create(QList<SharedVal> vs);
    QString toString();
};

}
}

#endif // QSCHEME_AST_OR_H
