#ifndef QUOTE_H
#define QUOTE_H

#include "val.h"

namespace qscheme {
namespace ast {

class Quote : public Val {
protected:
    SharedVal val;
    Quote(SharedVal v);

public:
    static SharedVal create(SharedVal v);
    QString toString();
};

}
}

#endif // QUOTE_H
