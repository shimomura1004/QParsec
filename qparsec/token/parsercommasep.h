#ifndef QPARSEC_TOKEN_PARSERCOMMASEP
#define QPARSEC_TOKEN_PARSERCOMMASEP

#include "parser.h"
#include "parsercomma.h"

namespace qparsec {
namespace token {

template<typename T>
Parser<QList<T>> *CommaSep(Parser<T> *p)
{ return SepBy(p, Comma()); }

template<typename T>
Parser<QList<T>> *CommaSep1(Parser<T> *p)
{ return SepBy1(p, Comma()); }

}
}

#endif // QPARSEC_TOKEN_PARSERCOMMASEP

