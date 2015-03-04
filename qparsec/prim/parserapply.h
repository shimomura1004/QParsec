#ifndef QPARSEC_PRIM_PARSERAPPLY
#define QPARSEC_PRIM_PARSERAPPLY

#include "parser.h"

namespace qparsec {
namespace prim {

template<typename T1, typename T2>
class ParserApply : public Parser<T2> {
protected:
    Parser<T1> *p_;
    T2 (*func_)(T1);

public:
    ParserApply(Parser<T1> *p, T2 (*func)(T1)) : Parser<T2>(), p_(p), func_(func) {}
    ~ParserApply() { delete p_; }

    T2 parse(Input &input) {
        return func_(p_->parse(input));
    }
};

template<typename T2>
class ParserApply<void, T2> : public Parser<T2> {
protected:
    Parser<void> *p_;
    T2 (*func_)();

public:
    ParserApply(Parser<void> *p, T2 (*func)()) : Parser<T2>(), p_(p), func_(func) {}
    ~ParserApply() { delete p_; }

    T2 parse(Input &input) {
        return p_->parse(input);
    }
};

template<typename T1, typename T2>
ParserApply<T1, T2> *Apply(Parser<T1> *p, T2(*func)(T1))
{ return new ParserApply<T1, T2>(p, func); }

}
}

#endif // QPARSEC_PRIM_PARSERAPPLY

