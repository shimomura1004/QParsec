#ifndef VAL_H
#define VAL_H

#include <QString>
#include <QSharedPointer>

namespace ast {

class Val {
public:
    virtual ~Val(){}
    virtual QString toString() = 0;
};

typedef QSharedPointer<Val> SharedVal;
typedef QList<QPair<QString, SharedVal>> Env;

}

#endif // VAL_H

