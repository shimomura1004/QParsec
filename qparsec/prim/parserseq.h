#ifndef PARSERSEQ
#define PARSERSEQ

#include "parser.h"

namespace qparsec {
namespace prim {

template<typename... Ts>
class ParserSeq : public Parser<void> {
public:
    void parse(Input&) {}
};

template<typename T, typename... Ts>
class ParserSeq<T, Ts...> : public ParserSeq<Ts...> {
    Parser<T>* p_;

public:
    ParserSeq(Parser<T>* p, Parser<Ts>*... ps) : ParserSeq<Ts...>(ps...), p_(p) {}
    ~ParserSeq() { delete p_; }

    void parse(Input &input) {
        p_->parse(input);
        ParserSeq<Ts...>::parse(input);
    }
};

template<typename... Ts>
ParserSeq<Ts...> *Seq(Parser<Ts>*... ps)
{ return new ParserSeq<Ts...>(ps...); }

}
}

#endif // PARSERSEQ

