#include "parserboolean.h"

#include "prim/parserapply.h"
#include "character/character.h"
#include "combinator/combinator.h"

namespace qparsec {
namespace language {
namespace scheme {

using namespace prim;
using namespace character;
using namespace combinator;

Parser<bool> *Boolean() {
    bool(*convertTrue)(QString) = [](QString s){ return s == "#t"; };
    bool(*convertFalse)(QString) = [](QString s){ return s != "#f"; };
    return Choice({Apply(Str("#t"), convertTrue), Apply(Str("#f"), convertFalse)});
}

}
}
}
