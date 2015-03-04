#ifndef QSCHEME_AST_VECTOR_H
#define QSCHEME_AST_VECTOR_H

#include "val.h"

namespace qscheme {
namespace ast {

class Vector : public Val {
protected:
    QList<SharedVal> elems;
    Vector(QList<SharedVal> e);

public:
    static SharedVal create(QList<SharedVal> e);
    QString toString();
};

}
}

#endif // QSCHEME_AST_VECTOR_H
