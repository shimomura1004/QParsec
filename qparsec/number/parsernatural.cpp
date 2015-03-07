#include "parsernatural.h"
#include "prim/prim.h"
#include "combinator/combinator.h"
#include "number.h"

namespace qparsec {
namespace number {
using namespace qparsec;
using namespace prim;
using namespace combinator;

Parser<int> *Natural() {
    return Choice({Try(Hexadecimal()), Try(Octal()), Try(Decimal())});
}

}
}
