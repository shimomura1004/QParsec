#include "parserselfevaluating.h"
#include "expression.h"
#include "parser/datum/datum.h"
#include "prim/prim.h"
#include "character/character.h"
#include "combinator/combinator.h"

namespace qscheme {
namespace parser {
namespace expression {
using namespace qparsec;
using namespace prim;
using namespace character;
using namespace combinator;

qparsec::Parser<ast::SharedVal> *SelfEvaluating() {
    return Choice({ Try(datum::Boolean()), Try(datum::Number()), Try(datum::Character()), datum::String() });
}

}
}
}
