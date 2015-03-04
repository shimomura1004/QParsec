#ifndef SET_H
#define SET_H

#include "val.h"

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

#endif // SET_H
