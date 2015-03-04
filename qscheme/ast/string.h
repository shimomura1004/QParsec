#ifndef QSCHEME_AST_STRING_H
#define QSCHEME_AST_STRING_H

#include "val.h"

namespace qscheme {
namespace ast {

class String : public Val {
protected:
    QString val;
    String(QString s);

public:
    static SharedVal create(QString s);
    QString toString();
};

}
}

#endif // QSCHEME_AST_STRING_H
