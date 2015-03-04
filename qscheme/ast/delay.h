#ifndef QSCHEME_AST_DELAY_H
#define QSCHEME_AST_DELAY_H

#include "val.h"

namespace qscheme {
namespace ast {

class Delay : public Val {
protected:
    SharedVal val;
    Delay(SharedVal v);

public:
    static SharedVal create(SharedVal v);
    QString toString();
};

}
}

#endif // QSCHEME_AST_DELAY_H
