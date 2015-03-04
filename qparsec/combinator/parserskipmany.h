#ifndef QPARSEC_COMBINATOR_PARSERSKIPMANY
#define QPARSEC_COMBINATOR_PARSERSKIPMANY

#include "parser.h"

namespace qparsec {
namespace combinator {

template<typename T>
class ParserSkipMany : public Parser<void> {
protected:
    Parser<T> *p_;

public:
    ParserSkipMany(Parser<T> *p) : p_(p) {}
    ~ParserSkipMany() {delete p_;}

    void parse(Input &input) {
        try {
            Q_FOREVER {
                p_->parse(input);
            }
        }
        catch (const ParserException &) {}
    }
};

template<typename T>
class ParserSkipMany1 : public ParserSkipMany<T> {
public:
    ParserSkipMany1(Parser<T> *p) : ParserSkipMany<T>(p) {}

    void parse(Input &input) {
        ParserSkipMany<T>::p_->parse(input);
        ParserSkipMany<T>::parse(input);
    }
};

template<typename T>
ParserSkipMany<T> *SkipMany(Parser<T> *p)
{ return new ParserSkipMany<T>(p); }

template<typename T>
ParserSkipMany1<T> *SkipMany1(Parser<T> *p)
{ return new ParserSkipMany1<T>(p); }

}
}

#endif // QPARSEC_COMBINATOR_PARSERSKIPMANY

