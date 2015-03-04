#ifndef QPARSEC_LANGUAGE_SCHEME_PARSERCHARACTER_H
#define QPARSEC_LANGUAGE_SCHEME_PARSERCHARACTER_H

#include "qparsec.h"
#include "prim/prim.h"
#include "character/character.h"
#include "combinator/combinator.h"

namespace qparsec {
namespace language {
namespace scheme {

class ParserCharacter : public Parser<QChar> {
protected:
    Parser<QString> *CharacterName();

public:
    QChar parse(Input &input);
};

Parser<QChar> *Character();

}
}
}

#endif // QPARSEC_LANGUAGE_SCHEME_PARSERCHARACTER_H
