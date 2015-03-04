#ifndef QSCHEME_AST_REAL_H
#define QSCHEME_AST_REAL_H

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

#endif // QSCHEME_AST_REAL_H
