#ifndef QPARSEC_PRIM_PARSERTRY
#define QPARSEC_PRIM_PARSERTRY

#include "parser.h"
#include "parserexception.h"

namespace qparsec {
namespace prim {

template<typename T>
class ParserTry : public Parser<T> {
protected:
    Parser<T> *p_;

public:
    ParserTry(Parser<T> *p) : p_(p) {}
    ~ParserTry() {delete p_;}

    T parse(Input &input) {
        input.preserve();
        try {
            auto result = p_->parse(input);
            input.clear();
            return result;
        }
        catch (const ParserException &e) {
            input.restore();
            throw e;
        }
    }
};

template<>
class ParserTry<void> : public Parser<void> {
protected:
    Parser<void> *p_;

public:
    ParserTry(Parser<void> *p);
    ~ParserTry();

    void parse(Input &input);
};

template<typename T>
ParserTry<T> *Try(Parser<T> *p)
{ return new ParserTry<T>(p); }

}
}

#endif // QPARSEC_PRIM_PARSERTRY

