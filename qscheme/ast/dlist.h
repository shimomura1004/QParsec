#ifndef DLIST_H
#define DLIST_H

#include "val.h"

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

#endif // DLIST_H
