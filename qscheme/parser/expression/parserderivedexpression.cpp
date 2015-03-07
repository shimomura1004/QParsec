#include "parserderivedexpression.h"
#include "expression.h"
#include "prim/prim.h"
#include "combinator/combinator.h"

namespace qscheme {
namespace parser {
namespace expression {
using namespace qparsec;
using namespace prim;
using namespace combinator;

ast::SharedVal ParserDerivedExpression::parse(Input &input) {
    return Choice({ Try(Cond()),
                    Try(Case()),
                    Try(And()),
                    Try(Or()),
                    Try(Let()),
                    Try(Begin()),
                    Try(Do()),
                    Try(Delay())
                  })->parse(input);
    // todo: support quasification
}

Parser<ast::SharedVal> *DerivedExpression() {
    return new ParserDerivedExpression();
}

}
}
}
