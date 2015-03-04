#ifndef QSCHEME_AST_INTEGER_H
#define QSCHEME_AST_INTEGER_H

#include "val.h"

namespace qscheme {
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
}

#endif // QSCHEME_AST_INTEGER_H
