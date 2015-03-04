#ifndef QPARSEC_PRIM_PARSERAPPLY2
#define QPARSEC_PRIM_PARSERAPPLY2

#include "parser.h"

namespace qparsec {
namespace prim {

template<typename T1, typename T2, typename T3>
class ParserApply2 : public Parser<T2> {
protected:
    Parser<T1> *p1_;
    Parser<T2> *p2_;
    T3(*func_)(T1, T2);

public:
    ParserApply2(Parser<T1> *p1, Parser<T2> *p2, T3 (*func)(T1, T2)) : Parser<T3>(), p1_(p1), p2_(p2), func_(func) {}
    ~ParserApply2() {
        delete p1_;
        delete p2_;
    }

    T3 parse(Input &input) {
        auto r1 = p1_->parse(input);
        auto r2 = p2_->parse(input);
        return func_(r1, r2);
    }
};

template<typename T1, typename T2, typename T3>
ParserApply2<T1, T2, T3> *Apply2(Parser<T1> *p1, Parser<T2> *p2, T3(*func)(T1, T2))
{ return new ParserApply2<T1, T2, T3>(p1, p2, func); }

}
}

#endif // QPARSEC_PRIM_PARSERAPPLY2

