#include "parserhexadecimal.h"
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

Parser<int> *Hexadecimal() {
    return Apply<QString, int>(
                Lexeme(Right(Seq(Char('0'), OneOf("xX")),
                             Many1(OneOf("0123456789abcdefABCDEF")))),
                [](QString s){return s.toInt(0, 16);});
}

}
}
