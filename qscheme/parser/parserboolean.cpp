#include "parserboolean.h"
#include "prim/prim.h"
#include "language/scheme/scheme.h"

namespace qscheme {
namespace parser {

using namespace qparsec;
using namespace prim;
using namespace language;

Parser<ast::SharedVal> *Boolean() {
    return Apply(scheme::Boolean(), ast::Bool::create);
}

}
}
