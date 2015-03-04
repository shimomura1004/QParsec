#ifndef INTEGER_H
#define INTEGER_H

#include "val.h"

namespace ast {

class Integer : public Val {
protected:
    int64_t val;
    Integer(int i);

public:
    static SharedVal create(int64_t i);
    QString toString();
};

}

#endif // INTEGER_H
