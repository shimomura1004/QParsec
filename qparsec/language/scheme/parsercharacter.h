#ifndef PARSERCHARACTER_H
#define PARSERCHARACTER_H

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

#endif // PARSERCHARACTER_H
