#ifndef REAL_H
#define REAL_H

#include "val.h"

namespace qscheme {
namespace ast {

class Real : public Val {
protected:
    double val;
    Real(double d);

public:
    static SharedVal create(double d);
    QString toString();
};

}
}

#endif // REAL_H
