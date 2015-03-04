#ifndef CHAR_H
#define CHAR_H

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

#endif // CHAR_H
