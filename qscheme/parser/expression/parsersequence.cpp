#include "parsersequence.h"
#include "expression.h"
#include "combinator/combinator.h"

namespace qscheme {
namespace parser {
namespace expression {
using namespace qparsec;
using namespace combinator;

Parser<QList<ast::SharedVal> > *Sequence() {
    return Many1(Expression());
}

}
}
}
