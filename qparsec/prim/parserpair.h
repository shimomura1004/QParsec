#ifndef QPARSEC_PRIM_PARSERPAIR
#define QPARSEC_PRIM_PARSERPAIR

#include "parser.h"
#include <QPair>

namespace qparsec {
namespace prim {

template<typename T1, typename T2>
class ParserPair : public Parser<QPair<T1, T2>> {
protected:
    Parser<T1> *p1_;
    Parser<T2> *p2_;

public:
    ParserPair(Parser<T1> *p1, Parser<T2> *p2) : Parser<QPair<T1, T2>>(), p1_(p1), p2_(p2) {}
    ~ParserPair() {
        delete p1_;
        delete p2_;
    }

    QPair<T1, T2> parse(Input &input) {
        T1 left = p1_->parse(input);
        T2 right = p2_->parse(input);
        return QPair<T1, T2>(left, right);
    }
};

template<typename T1, typename T2>
ParserPair<T1, T2> *Pair(Parser<T1> *p1, Parser<T2> *p2)
{ return new ParserPair<T1, T2>(p1, p2); }

}
}

#endif // QPARSEC_PRIM_PARSERPAIR

