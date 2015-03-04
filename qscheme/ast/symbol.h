#ifndef QSCHEME_AST_SYMBOL_H
#define QSCHEME_AST_SYMBOL_H

#include "val.h"

namespace qscheme {
namespace ast {

class Symbol : public Val {
protected:
    QString val;
    Symbol(QString s);

public:
    static SharedVal create(QString s);
    QString toString();
};

}
}

#endif // QSCHEME_AST_SYMBOL_H
