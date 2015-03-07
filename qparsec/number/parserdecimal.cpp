#include "parserdecimal.h"
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

Parser<int> *Decimal() {
    return Apply<QString, int>(Lexeme(Many1(Digit())), [](QString s){return s.toInt();});
}

}
}
