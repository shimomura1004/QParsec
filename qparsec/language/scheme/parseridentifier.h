#ifndef QPARSEC_LANGUAGE_SCHEME_PARSERIDENTIFIER_H
#define QPARSEC_LANGUAGE_SCHEME_PARSERIDENTIFIER_H

#include <QString>
#include "qparsec.h"
#include "prim/prim.h"
#include "character/character.h"
#include "combinator/combinator.h"

namespace qparsec {
namespace language {
namespace scheme {

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

#endif // QPARSEC_LANGUAGE_SCHEME_PARSERIDENTIFIER_H
