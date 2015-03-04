#ifndef QSCHEME_AST_BOOL_H
#define QSCHEME_AST_BOOL_H

#include "val.h"

namespace qscheme {
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
}

#endif // QSCHEME_AST_BOOL_H
