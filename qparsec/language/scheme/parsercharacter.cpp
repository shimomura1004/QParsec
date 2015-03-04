#include "parsercharacter.h"

namespace qparsec {
namespace language {
namespace scheme {

using namespace qparsec;
using namespace prim;
using namespace character;
using namespace combinator;

Parser<QString> *ParserCharacter::CharacterName() { return Choice({Str("space"), Str("newline")}); }

QChar ParserCharacter::parse(Input &input) {
    Str("#\\")->parse(input);

    try {
        auto name = Try(CharacterName())->parse(input);
        if (name == "space")
            return QChar(' ');
        if (name == "newline")
            return QChar('\n');
    }
    catch (const ParserException &) {}

    auto c = AnyChar()->parse(input);
    return QChar(c);
}

Parser<QChar> *Character() { return new ParserCharacter(); }

}
}
}
