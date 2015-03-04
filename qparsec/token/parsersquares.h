#ifndef PARSERSQUARES
#define PARSERSQUARES

#include "parser.h"
#include "combinator/combinator.h"
#include "parsersymbol.h"

namespace qparsec {
namespace token {

using namespace combinator;

template<typename T>
Parser<T> *Squares(Parser<T> *p)
{ return Between(p, Symbol("["), Symbol("]")); }

}
}
#endif // PARSERSQUARES

