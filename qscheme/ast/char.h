#ifndef QSCHEME_AST_CHAR_H
#define QSCHEME_AST_CHAR_H

#include "val.h"

namespace qscheme {
namespace ast {

class Char : public Val {
protected:
    QChar val;
    Char(QChar c);

public:
    static SharedVal create(QChar c);
    QString toString();
};

}
}

#endif // QSCHEME_AST_CHAR_H
