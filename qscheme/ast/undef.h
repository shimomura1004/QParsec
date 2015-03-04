#ifndef QSCHEME_AST_UNDEF_H
#define QSCHEME_AST_UNDEF_H

#include "val.h"

namespace qscheme {
namespace ast {

class Undef : public Val {
public:
    static SharedVal create();
    QString toString();
};

}
}

#endif // QSCHEME_AST_UNDEF_H
