#include "parsercount.h"

namespace qparsec {
namespace combinator {

ParserCount<QChar> *Count(Parser<QChar> *p, int n)
{ return new ParserCount<QChar>(p, n); }

}
}
