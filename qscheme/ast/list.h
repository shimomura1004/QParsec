#ifndef QSCHEME_AST_LIST_H
#define QSCHEME_AST_LIST_H

#include "val.h"

namespace qscheme {
namespace ast {

class List : public Val {
protected:
    QList<SharedVal> val;
    List(QList<SharedVal> l);

public:
    static SharedVal create(QList<SharedVal> l);
    QString toString();
};

}
}

#endif // QSCHEME_AST_LIST_H
