#ifndef PARSERENDBY
#define PARSERENDBY

#include "parser.h"

namespace qparsec {
namespace combinator {

template<typename T, typename TSep>
class ParserEndBy : public Parser< QList<T> > {
protected:
    Parser<T> *p_;
    Parser<TSep> *sep_;

public:
    ParserEndBy(Parser<T> *p, Parser<TSep> *sep) : Parser< QList<T> >(), p_(p), sep_(sep) {}
    ~ParserEndBy() {
        delete p_;
        delete sep_;
    }

    QList<T> parse(Input &input) {
        QList<T> result;

        Q_FOREVER {
            try {
                result.push_back(p_->parse(input));
            }
            catch (const ParserException &) {
                return result;
            }
            sep_->parse(input);
        }
    }
};

template<typename T, typename TSep>
class ParserEndBy1 : public Parser< QList<T> > {
protected:
    Parser<T> *p_;
    Parser<TSep> *sep_;

public:
    ParserEndBy1(Parser<T> *p, Parser<TSep> *sep) : Parser< QList<T> >(), p_(p), sep_(sep) {}
    ~ParserEndBy1() {
        delete p_;
        delete sep_;
    }

    QList<T> parse(Input &input) {
        QList<T> result;

        result.push_back(p_->parse(input));
        sep_->parse(input);

        Q_FOREVER {
            try {
                result.push_back(p_->parse(input));
            }
            catch (const ParserException &) {
                return result;
            }
            sep_->parse(input);
        }
    }
};

template<typename T, typename TSep>
ParserEndBy<T, TSep> *EndBy(Parser<T> *p, Parser<TSep> *psep)
{ return new ParserEndBy<T, TSep>(p, psep); }

template<typename T, typename TSep>
ParserEndBy1<T, TSep> *EndBy1(Parser<T> *p, Parser<TSep> *psep)
{ return new ParserEndBy1<T, TSep>(p, psep); }

}
}

#endif // PARSERENDBY

