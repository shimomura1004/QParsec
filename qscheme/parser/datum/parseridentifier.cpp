#include "parseridentifier.h"
#include "prim/prim.h"
#include "language/scheme/scheme.h"

namespace qscheme {
namespace parser {
namespace datum {

using namespace qparsec;
using namespace prim;
using namespace language;

qparsec::Parser<ast::SharedVal> *Identifier() {
    return Apply(scheme::Identifier(), ast::Symbol::create);
}

}
}
}
