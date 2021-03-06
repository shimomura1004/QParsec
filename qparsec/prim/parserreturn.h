#ifndef QPARSEC_PRIM_PARSERRETURN_H
#define QPARSEC_PRIM_PARSERRETURN_H

#include <parser.h>
#include <input.h>

namespace qparsec {
namespace prim {

template<typename T>
class ParserReturn : public Parser<T>
{
protected:
    T value_;

public:
    ParserReturn(T value) : value_(value) {}

    T parse(Input &) {
        return value_;
    }
};

template<typename T>
ParserReturn<T> *Return(T v)
{ return new ParserReturn<T>(v); }

}
}

#endif // QPARSEC_PRIM_PARSERRETURN_H
