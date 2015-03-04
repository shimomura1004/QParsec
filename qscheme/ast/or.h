#ifndef OR_H
#define OR_H

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

#endif // OR_H
