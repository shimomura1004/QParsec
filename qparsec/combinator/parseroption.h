#ifndef PARSEROPTION
#define PARSEROPTION

#include "parser.h"

namespace qparsec {
namespace combinator {

template<typename T>
class ParserOption : public Parser<T> {
protected:
    Parser<T> *p_;
    T opt_;

public:
    ParserOption(Parser<T> *p, T opt) : Parser<T>(), p_(p), opt_(opt) {}
    ~ParserOption() { delete p_; }

    T parse(Input &input) {
        try {
            T result = p_->parse(input);
            return result;
        }
        catch (const ParserException &) {
            return opt_;
        }
    }
};
template<typename T>
class ParserOption_ : public Parser<void> {
protected:
    Parser<T> *p_;

public:
    ParserOption_(Parser<T> *p) : Parser<void>(), p_(p) {}
    ~ParserOption_() { delete p_; }

    void parse(Input &input) {
        try { p_->parse(input); }
        catch (const ParserException &) {}
    }
};

template<typename T>
ParserOption<T> *Option(Parser<T> *p, T opt)
{ return new ParserOption<T>(p, opt); }

template<typename T>
Parser<void> *Option_(Parser<T> *p)
{ return new ParserOption_<T>(p); }

}
}

#endif // PARSEROPTION

