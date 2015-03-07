#include "parseroctal.h"
#include "prim/prim.h"
#include "character/character.h"
#include "combinator/combinator.h"
#include "token/token.h"

namespace qparsec {
namespace number {
using namespace qparsec;
using namespace prim;
using namespace character;
using namespace combinator;
using namespace token;

Parser<int> *Octal() {
    return Apply<QString, int>(
                Lexeme(Right(Seq(Char('0'), OneOf("oO")),
                             Many1(OneOf("01234567")))),
                [](QString s){return s.toInt(0, 8);});
}

}
}
