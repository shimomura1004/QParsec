#ifndef QPARSEC_TOKEN_PARSERSQUARES
#define QPARSEC_TOKEN_PARSERSQUARES

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
#endif // QPARSEC_TOKEN_PARSERSQUARES

