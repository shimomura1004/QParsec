#ifndef QPARSEC_PRIM_PARSERLAZY
#define QPARSEC_PRIM_PARSERLAZY

#include "parser.h"

namespace qparsec {
namespace prim {

template<typename T>
class ParserLazy : public Parser<T> {
protected:
    Parser<T>*(*p_)();

public:
    ParserLazy(Parser<T>* (*p)()) : Parser<T>(), p_(p) {}
    T parse(Input &input) {
        return p_()->parse(input);
    }
};

template<typename T>
ParserLazy<T> *Lazy(Parser<T>* (*p)())
{ return new ParserLazy<T>(p); }

}
}

#endif // QPARSEC_PRIM_PARSERLAZY

