#include "parserliteral.h"
#include "expression.h"
#include "prim/prim.h"
#include "combinator/combinator.h"

namespace qscheme {
namespace parser {
namespace expression {
using namespace qparsec;
using namespace prim;
using namespace combinator;

Parser<ast::SharedVal> *Literal() {
    return Choice({ Try(Quotation()), SelfEvaluating()});
}

}
}
}
