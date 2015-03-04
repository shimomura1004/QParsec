#ifndef PARSERPARENS_H
#define PARSERPARENS_H

#include "parser.h"
#include "combinator/combinator.h"
#include "parsersymbol.h"

namespace qparsec {
namespace token {

using namespace combinator;

template<typename T>
Parser<T> *Parens(Parser<T> *p)
{ return Between(p, Symbol("("), Symbol(")")); }

}
}

#endif // PARSERPARENS_H
