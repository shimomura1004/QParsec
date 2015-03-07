#include "parserbody.h"
#include "expression.h"

namespace qscheme {
namespace parser {
namespace expression {
using namespace qparsec;

Parser<QList<ast::SharedVal> > *Body() {
    // todo: support definition inside lambda
    // Many(Definition())
    return Sequence();
}

}
}
}
