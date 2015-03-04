#ifndef QPARSEC_CHARACTER_PARSERSTR_H
#define QPARSEC_CHARACTER_PARSERSTR_H

#include "parser.h"
#include "parserexception.h"

namespace qparsec {
namespace character {

class ParserStr : public Parser<QString> {
protected:
    QString s_;

public:
    ParserStr(QString s);
    QString parse(Input &input);
};

ParserStr *Str(QString s);

}
}

#endif // QPARSEC_CHARACTER_PARSERSTR_H
