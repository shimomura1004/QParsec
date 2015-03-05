#include "parsercompounddatum.h"
#include "combinator/combinator.h"
#include "datum.h"

namespace qscheme {
namespace parser {
namespace datum {
using namespace qparsec;
using namespace combinator;

Parser<ast::SharedVal> *CompoundDatum() {
    return Choice({ List(), Vector() });
}

}
}
}
