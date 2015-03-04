#ifndef QSCHEME_AST_DEFINE_H
#define QSCHEME_AST_DEFINE_H

#include "val.h"

namespace qscheme {
namespace ast {

class Define : public Val {
protected:
    QString name;
    SharedVal body;
    Define(QString n, SharedVal b);

public:
    static SharedVal create(QString n, SharedVal b);
    QString toString();
};

}
}

#endif // QSCHEME_AST_DEFINE_H
