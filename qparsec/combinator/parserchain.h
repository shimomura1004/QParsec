#ifndef PARSERCHAIN
#define PARSERCHAIN

#include "parser.h"
#include "parserexception.h"

namespace qparsec {
namespace combinator {

template<typename T>
class ParserChainl : public Parser<T> {
protected:
    Parser<T> *p_;
    Parser<T(*)(T, T)> *poperator_;
    T opt_;

public:
    ParserChainl(Parser<T> *p, Parser<T(*)(T, T)> *poperator, T opt) : Parser<T>(), p_(p), poperator_(poperator), opt_(opt) {}
    ~ParserChainl() {
        delete p_;
        delete poperator_;
    }

    T parse(Input &input) {
        T accum;
        T(*op)(T, T);

        try {
            accum = p_->parse(input);
        }
        catch (const ParserException &) {
            return opt_;
        }

        Q_FOREVER {
            try {
                op = poperator_->parse(input);
            }
            catch (const ParserException &) {
                return accum;
            }

            T t = p_->parse(input);
            accum = op(accum, t);
        }
    }
};

template<typename T>
class ParserChainl1 : public Parser<T> {
protected:
    Parser<T> *p_;
    Parser<T(*)(T, T)> *poperator_;

public:
    ParserChainl1(Parser<T> *p, Parser<T(*)(T, T)> *poperator) : Parser<T>(), p_(p), poperator_(poperator) {}
    ~ParserChainl1() {
        delete p_;
        delete poperator_;
    }

    T parse(Input &input) {
        T accum = p_->parse(input);
        T(*op)(T, T);

        Q_FOREVER {
            try {
                op = poperator_->parse(input);
            }
            catch (const ParserException &) {
                return accum;
            }

            T t = p_->parse(input);
            accum = op(accum, t);
        }
    }
};

template<typename T>
class ParserChainr : public Parser<T> {
protected:
    Parser<T> *p_;
    Parser<T(*)(T, T)> *poperator_;
    T opt_;

public:
    ParserChainr(Parser<T> *p, Parser<T(*)(T, T)> *poperator, T opt) : Parser<T>(), p_(p), poperator_(poperator), opt_(opt) {}
    ~ParserChainr() {
        delete p_;
        delete poperator_;
    }

    T parse(Input &input) {
        T left;
        T(*op)(T, T);

        try {
            left = p_->parse(input);
        }
        catch (const ParserException &) {
            return opt_;
        }

        try {
            op = poperator_->parse(input);
        }
        catch (const ParserException &) {
            return left;
        }

        return op(left, parse(input));
    }
};

template<typename T>
class ParserChainr1 : public Parser<T> {
protected:
    Parser<T> *p_;
    Parser<T(*)(T, T)> *poperator_;

public:
    ParserChainr1(Parser<T> *p, Parser<T(*)(T, T)> *poperator) : Parser<T>(), p_(p), poperator_(poperator) {}
    ~ParserChainr1() {
        delete p_;
        delete poperator_;
    }

    T parse(Input &input) {
        T left;
        T(*op)(T, T);

        left = p_->parse(input);

        try {
            op = poperator_->parse(input);
        }
        catch (const ParserException &) {
            return left;
        }

        return op(left, parse(input));
    }
};

template<typename T>
ParserChainl<T> *Chainl(Parser<T> *p, Parser<T(*)(T, T)> *poperator, T opt)
{ return new ParserChainl<T>(p, poperator, opt); }

template<typename T>
ParserChainl1<T> *Chainl1(Parser<T> *p, Parser<T(*)(T, T)> *poperator)
{ return new ParserChainl1<T>(p, poperator); }

template<typename T>
ParserChainr<T> *Chainr(Parser<T> *p, Parser<T(*)(T, T)> *poperator, T opt)
{ return new ParserChainr<T>(p, poperator, opt); }

template<typename T>
ParserChainr1<T> *Chainr1(Parser<T> *p, Parser<T(*)(T, T)> *poperator)
{ return new ParserChainr1<T>(p, poperator); }

}
}

#endif // PARSERCHAIN

