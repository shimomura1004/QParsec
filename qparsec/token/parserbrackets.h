#ifndef PARSERBRACKETS
#define PARSERBRACKETS

#include "parser.h"
#include "combinator/combinator.h"
#include "parserlexeme.h"

namespace qparsec {
namespace token {

using namespace combinator;

template<typename T>
Parser<T> *Brackets(Parser<T> *p)
{ return Between(p, Symbol("<"), Symbol(">")); }

}
}

#endif // PARSERBRACKETS

