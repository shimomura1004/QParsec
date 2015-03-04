#include "parserboolean.h"
#include "prim/prim.h"
#include "language/scheme/scheme.h"

namespace qscheme {
namespace parser {

qparsec::Parser<ast::SharedVal> *Boolean() {
    //return qparsec::prim::Apply(qparsec::language::scheme::Boolean(), ast::Bool::create);
}

}
}
