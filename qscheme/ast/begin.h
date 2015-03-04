#ifndef BEGIN_H
#define BEGIN_H

#include "val.h"

namespace ast {

class Begin : public Val {
protected:
    QList<SharedVal> vals;
    Begin(QList<SharedVal> v);

public:
    static SharedVal create(QList<SharedVal> v);
    QString toString();
};

}

#endif // BEGIN_H
