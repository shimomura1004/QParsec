#ifndef QSCHEME_AST_DLIST_H
#define QSCHEME_AST_DLIST_H

#include "val.h"

namespace qscheme {
namespace ast {

class DList : public Val {
protected:
    QList<SharedVal> cars;
    SharedVal cdr;
    DList(QList<SharedVal> a, SharedVal d);

public:
    static SharedVal create(QList<SharedVal> l, SharedVal d);
    QString toString();
};

}
}

#endif // QSCHEME_AST_DLIST_H
