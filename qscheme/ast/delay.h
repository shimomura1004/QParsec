#ifndef DELAY_H
#define DELAY_H

#include "val.h"

namespace ast {

class Delay : public Val {
protected:
    SharedVal val;
    Delay(SharedVal v);

public:
    static SharedVal create(SharedVal v);
    QString toString();
};

}

#endif // DELAY_H
