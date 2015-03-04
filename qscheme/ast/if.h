#ifndef IF_H
#define IF_H

#include "val.h"

namespace ast {

class If : public Val {
protected:
    SharedVal condition;
    SharedVal left;
    SharedVal right;
    If(SharedVal c, SharedVal l, SharedVal r);

public:
    static SharedVal create(SharedVal c, SharedVal l, SharedVal r);
    QString toString();
};

}
#endif // IF_H
