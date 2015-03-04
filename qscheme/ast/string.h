#ifndef STRING_H
#define STRING_H

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

#endif // STRING_H
