#ifndef PARSERBRACES_H
#define PARSERBRACES_H

#include "parser.h"
#include "combinator/combinator.h"
#include "parsersymbol.h"

namespace qparsec {
namespace token {

using namespace combinator;

template<typename T>
Parser<T> *Braces(Parser<T> *p)
{ return Between(p, Symbol("{"), Symbol("}")); }

}
}

#endif // PARSERBRACES_H
