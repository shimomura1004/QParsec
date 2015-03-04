#ifndef QSCHEME_AST_RATIONAL_H
#define QSCHEME_AST_RATIONAL_H

#include "val.h"

namespace qscheme {
namespace ast {

class Rational : public Val {
protected:
    int64_t numerator;
    uint64_t denominator;
    Rational(int64_t n, uint64_t d);

public:
    static SharedVal create(int64_t n, uint64_t d);
    QString toString();
};

}
}

#endif // QSCHEME_AST_RATIONAL_H
