#ifndef PARSERLEFT
#define PARSERLEFT

#include "parser.h"

namespace qparsec {

template<typename T1, typename T2>
class ParserLeft : public Parser<T1> {
protected:
    Parser<T1> *p1_;
    Parser<T2> *p2_;

public:
    ParserLeft(Parser<T1> *p1, Parser<T2> *p2) : Parser<T1>(), p1_(p1), p2_(p2) {}
    ~ParserLeft() {
        delete p1_;
        delete p2_;
    }

    T1 parse(Input &input) {
        T1 result = p1_->parse(input);
        p2_->parse(input);
        return result;
    }
};

template<typename T1, typename T2>
ParserLeft<T1, T2> *Left(Parser<T1> *p1, Parser<T2> *p2)
{ return new ParserLeft<T1, T2>(p1, p2); }

}

#endif // PARSERLEFT

