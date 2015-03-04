#ifndef VARIABLE_H
#define VARIABLE_H

#include "val.h"

namespace qscheme {
namespace ast {

class Variable : public Val {
protected:
    QString val;
    Variable(QString s);

public:
    static SharedVal create(QString s);
    QString toString();
};

}
}

#endif // VARIABLE_H
