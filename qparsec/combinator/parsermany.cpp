#include "parsermany.h"

namespace qparsec {
namespace combinator {

ParserMany<QChar> *Many(Parser<QChar> *p)
{ return new ParserMany<QChar>(p); }

ParserMany1<QChar> *Many1(Parser<QChar> *p)
{ return new ParserMany1<QChar>(p); }

}
}
