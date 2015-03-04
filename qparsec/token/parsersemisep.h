#ifndef PARSERSEMISEP
#define PARSERSEMISEP

#include "parser.h"
#include "parsersemi.h"

namespace qparsec {
namespace token {

template<typename T>
Parser<QList<T>> *SemiSep(Parser<T> *p)
{ return SepBy(p, Semi()); }

template<typename T>
Parser<QList<T>> *SemiSep1(Parser<T> *p)
{ return SepBy1(p, Semi()); }

}
}

#endif // PARSERSEMISEP

