#ifndef PARSRELOOKAHEAD
#define PARSRELOOKAHEAD

#include "parser.h"

namespace qparsec {
namespace combinator {

template<typename T>
class ParserLookAhead : public Parser<T> {
protected:
    Parser<T> *p_;

public:
    ParserLookAhead(Parser<T> *p) : Parser<T>(), p_(p) {}
    ~ParserLookAhead() { delete p_; }

    T parse(Input &input) {
        input.preserve();
        auto result = p_->parse(input);
        input.restore();
        return result;
    }
};

template<typename T>
ParserLookAhead<T> *LookAhead(Parser<T> *p)
{ return new ParserLookAhead<T>(p); }

}
}

#endif // PARSRELOOKAHEAD

