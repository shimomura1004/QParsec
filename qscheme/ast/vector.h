#ifndef VECTOR_H
#define VECTOR_H

#include "val.h"

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

#endif // VECTOR_H
