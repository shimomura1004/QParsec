#include "parsersimpledatum.h"
#include "prim/prim.h"
#include "combinator/combinator.h"
#include "datum.h"

namespace qscheme {
namespace parser {
namespace datum {

using namespace qparsec;
using namespace prim;
using namespace combinator;

Parser<qscheme::ast::SharedVal> *SimpleDatum() {
    return Choice({ Try(Boolean()),
                    Try(Number()),
                    Character(),
                    String(),
                    Identifier()
                  });
}

}
}
}
