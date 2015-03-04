#ifndef QPARSEC_PRIM_PARSERRIGHT
#define QPARSEC_PRIM_PARSERRIGHT

#include "parser.h"

namespace qparsec {
namespace prim {

template<typename T1, typename T2>
class ParserRight : public Parser<T2> {
protected:
    Parser<T1> *p1_;
    Parser<T2> *p2_;

public:
    ParserRight(Parser<T1> *p1, Parser<T2> *p2) : Parser<T2>(), p1_(p1), p2_(p2) {}
    ~ParserRight() {
        delete p1_;
        delete p2_;
    }

    T2 parse(Input &input) {
        p1_->parse(input);
        T2 result = p2_->parse(input);
        return result;
    }
};

template<typename T1, typename T2>
ParserRight<T1, T2> *Right(Parser<T1> *p1, Parser<T2> *p2)
{ return new ParserRight<T1, T2>(p1, p2); }

}
}

#endif // QPARSEC_PRIM_PARSERRIGHT

