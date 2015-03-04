#ifndef QPARSEC_COMBINATOR_PARSERBETWEEN
#define QPARSEC_COMBINATOR_PARSERBETWEEN

#include "parser.h"

namespace qparsec {
namespace combinator {

template<typename T, typename TOpen, typename TClose>
class ParserBetween : public Parser<T> {
protected:
    Parser<T> *p_;
    Parser<TOpen> *popen_;
    Parser<TClose> *pclose_;

public:
    ParserBetween(Parser<T> *p, Parser<TOpen> *popen, Parser<TClose> *pclose) : Parser<T>(), p_(p), popen_(popen), pclose_(pclose) {}
    ~ParserBetween() {
        delete p_;
        delete popen_;
        delete pclose_;
    }

    T parse(Input &input) {
        popen_->parse(input);
        T result = p_->parse(input);
        pclose_->parse(input);

        return result;
    }
};

template<typename T, typename TOpen, typename TClose>
ParserBetween<T, TOpen, TClose> *Between(Parser<T> *p, Parser<TOpen> *popen, Parser<TClose> *pclose)
{ return new ParserBetween<T, TOpen, TClose>(p, popen, pclose); }

}
}

#endif // QPARSEC_COMBINATOR_PARSERBETWEEN

