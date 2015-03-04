#ifndef PARSERSEPBY
#define PARSERSEPBY

#include "parser.h"

namespace qparsec {
namespace combinator {

template<typename T, typename TSep>
class ParserSepBy : public Parser< QList<T> > {
protected:
    Parser<T> *p_;
    Parser<TSep> *sep_;

public:
    ParserSepBy(Parser<T> *p, Parser<TSep> *sep) : Parser< QList<T> >(), p_(p), sep_(sep) {}
    ~ParserSepBy() {
        delete p_;
        delete sep_;
    }

    QList<T> parse(Input &input) {
        QList<T> result;

        try {
            result.push_back(p_->parse(input));
        }
        catch (const ParserException &) {
            return result;
        }

        Q_FOREVER {
            try {
                sep_->parse(input);
            }
            catch (const ParserException &) {
                return result;
            }

            result.push_back(p_->parse(input));
        }
    }
};

template<typename T, typename TSep>
class ParserSepBy1 : public Parser< QList<T> > {
protected:
    Parser<T> *p_;
    Parser<TSep> *sep_;

public:
    ParserSepBy1(Parser<T> *p, Parser<TSep> *sep) : Parser< QList<T> >(), p_(p), sep_(sep) {}
    ~ParserSepBy1() {
        delete p_;
        delete sep_;
    }

    QList<T> parse(Input &input) {
        QList<T> result;

        result.push_back(p_->parse(input));
        Q_FOREVER {
            try {
                sep_->parse(input);
            }
            catch (const ParserException &) {
                return result;
            }

            result.push_back(p_->parse(input));
        }
    }
};

template<typename T, typename TSep>
ParserSepBy<T, TSep> *SepBy(Parser<T> *p, Parser<TSep> *psep)
{ return new ParserSepBy<T, TSep>(p, psep); }

template<typename T, typename TSep>
ParserSepBy1<T, TSep> *SepBy1(Parser<T> *p, Parser<TSep> *psep)
{ return new ParserSepBy1<T, TSep>(p, psep); }

}
}

#endif // PARSERSEPBY

