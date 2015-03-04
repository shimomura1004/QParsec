#ifndef BOOL_H
#define BOOL_H

#include "val.h"

namespace ast {

class Bool : public Val {
protected:
    bool val;
    Bool(bool b);

public:
    static SharedVal create(bool b);
    QString toString();
};

}

#endif // BOOL_H
