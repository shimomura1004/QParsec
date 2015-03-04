#ifndef PARSERLAZY
#define PARSERLAZY

#include "parser.h"

namespace qparsec {
namespace prim {

template<typename T>
class ParserLazy : public Parser<T> {
    friend ParserLazy<T> *Lazy(Parser<T>* (*p)());
protected:
    Parser<T>*(*p_)();

    ParserLazy(Parser<T>* (*p)()) : Parser<T>(), p_(p) {}
public:

    T parse(Input &input) {
        return p_()->parse(input);
    }
};

template<typename T>
ParserLazy<T> *Lazy(Parser<T>* (*p)())
{ return new ParserLazy<T>(p); }

}
}

#endif // PARSERLAZY

