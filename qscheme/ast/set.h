#ifndef QSCHEME_AST_SET_H
#define QSCHEME_AST_SET_H

#include "val.h"

namespace qscheme {
namespace ast {

class Set : public Val {
protected:
    QString var;
    SharedVal exp;
    Set(QString v, SharedVal e);

public:
    static SharedVal create(QString v, SharedVal e);
    QString toString();
};

}
}

#endif // QSCHEME_AST_SET_H
