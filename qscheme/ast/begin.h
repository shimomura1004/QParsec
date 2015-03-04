#ifndef QSCHEME_AST_BEGIN_H
#define QSCHEME_AST_BEGIN_H

#include "val.h"

namespace qscheme {
namespace ast {

class Begin : public Val {
protected:
    QList<SharedVal> vals;
    Begin(QList<SharedVal> v);

public:
    static SharedVal create(QList<SharedVal> v);
    QString toString();
};

}
}

#endif // QSCHEME_AST_BEGIN_H
