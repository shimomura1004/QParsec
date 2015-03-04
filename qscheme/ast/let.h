#ifndef QSCHEME_AST_LET_H
#define QSCHEME_AST_LET_H

#include <QPair>
#include "val.h"

namespace qscheme {
namespace ast {

class Let : public Val {
protected:
    enum Type { Normal, Star, Rec };
    static const char* typeToString(Type type);

    QString name;
    QList<QPair<QString, SharedVal>> bindings;
    QList<SharedVal> sequence;
    Type type;

    Let(QString n, QList<QPair<QString, SharedVal>> b, QList<SharedVal> s, Type t);

public:
    static SharedVal create(QString n, QList<QPair<QString, SharedVal>> b, QList<SharedVal> s, Type t);

    QString toString();
};

}
}

#endif // QSCHEME_AST_LET_H
