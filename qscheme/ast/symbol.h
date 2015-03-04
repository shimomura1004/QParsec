#ifndef SYMBOL_H
#define SYMBOL_H

#include "val.h"

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

#endif // SYMBOL_H
