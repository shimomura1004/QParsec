#ifndef AND_H
#define AND_H

#include "val.h"

namespace qscheme {
namespace ast {

class And : public Val {
protected:
    QList<SharedVal> vals;
    And(QList<SharedVal> vs);

public:
    static SharedVal create(QList<SharedVal> vs);
    QString toString();
};

}
}

#endif // AND_H
