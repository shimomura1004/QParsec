#include "parserdatum.h"
#include "prim/prim.h"
#include "combinator/combinator.h"
#include "token/token.h"
#include "datum.h"

namespace qscheme {
namespace parser {
namespace datum {
using namespace qparsec;
using namespace prim;
using namespace combinator;
using namespace token;

ast::SharedVal ParserDatum::parse(Input &input) {
    return Lexeme(Choice({Try(CompoundDatum()), SimpleDatum()}))->parse(input);
}

qparsec::Parser<ast::SharedVal> *Datum() {
    return new ParserDatum();
}

}
}
}
