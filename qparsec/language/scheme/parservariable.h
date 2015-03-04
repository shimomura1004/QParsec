#ifndef PARSERVARIABLE_H
#define PARSERVARIABLE_H

#include <QStringList>
#include "qparsec.h"
#include "scheme.h"

namespace qparsec {
namespace language {
namespace scheme {

using namespace qparsec;

class ParserVariable : public Parser<QString> {
protected:
    static const QStringList ExpressionKeyword;
    static const QStringList SyntacticKeyword;

public:
    QString parse(Input &input);
};
Parser<QString> *Variable();

}
}
}

#endif // PARSERVARIABLE_H
