#ifndef QSCHEME_AST_IF_H
#define QSCHEME_AST_IF_H

#include "val.h"

namespace qscheme {
namespace ast {

class If : public Val {
protected:
    SharedVal condition;
    SharedVal left;
    SharedVal right;
    If(SharedVal c, SharedVal l, SharedVal r);

public:
    static SharedVal create(SharedVal c, SharedVal l, SharedVal r);
    QString toString();
};

}
}

#endif // QSCHEME_AST_IF_H
