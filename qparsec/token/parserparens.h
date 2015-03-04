#ifndef QPARSEC_TOKEN_PARSERPARENS_H
#define QPARSEC_TOKEN_PARSERPARENS_H

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

#endif // QPARSEC_TOKEN_PARSERPARENS_H
