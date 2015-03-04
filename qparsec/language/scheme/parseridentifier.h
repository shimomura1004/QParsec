#ifndef PARSERIDENTIFIER_H
#define PARSERIDENTIFIER_H

#include <QString>
#include "qparsec.h"
#include "prim/prim.h"
#include "character/character.h"
#include "combinator/combinator.h"

namespace qparsec {
namespace language {
namespace scheme {

using namespace qparsec;
using namespace prim;
using namespace character;
using namespace combinator;

class ParserIdentifier : public Parser<QString> {
protected:
    Parser<QChar> *Initial();
    Parser<QChar> *SpecialInitial();
    Parser<QChar> *Subsequent();
    Parser<QChar> *SpecialSubsequent();
    Parser<QString> *PeculiarIdentifier();

public:
    ParserIdentifier();
    QString parse(Input &input);
};

Parser<QString> *Identifier();

}
}
}

#endif // PARSERIDENTIFIER_H
