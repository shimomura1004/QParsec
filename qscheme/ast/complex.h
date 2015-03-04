#ifndef QSCHEME_AST_COMPLEX_H
#define QSCHEME_AST_COMPLEX_H

#include "val.h"

namespace qscheme {
namespace ast {

class Complex : public Val {
protected:
    double real;
    double imagnary;
    Complex(double r, double i);

public:
    static SharedVal create(double r, double i);
    QString toString();
};

}
}

#endif // QSCHEME_AST_COMPLEX_H
