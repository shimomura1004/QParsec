#ifndef UNDEF_H
#define UNDEF_H

#include "val.h"

namespace ast {

class Undef : public Val {
public:
    static SharedVal create();
    QString toString();
};

}

#endif // UNDEF_H
