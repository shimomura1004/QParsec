#ifndef PARSERMANY
#define PARSERMANY

#include "parser.h"
#include "parserexception.h"

namespace qparsec {
namespace combinator {

template<typename T>
class ParserMany : public Parser< QList<T> > {
protected:
    Parser<T> *p_;

public:
    ParserMany(Parser<T> *p) : Parser< QList<T> >(), p_(p) {}
    ~ParserMany() {delete p_;}

    QList<T> parse(Input &input) {
        QList<T> result;
        try {
            Q_FOREVER {
                result.push_back(p_->parse(input));
            }
        }
        catch (const ParserException &) {
            return result;
        }
    }
};

template<>
class ParserMany<QChar> : public Parser<QString> {
protected:
    Parser<QChar> *p_;

public:
    ParserMany(Parser<QChar> *p);
    ~ParserMany();
    QString parse(Input &input);
};

template<typename T>
class ParserMany1 : public ParserMany<T> {
public:
    ParserMany1(Parser<T> *p) : ParserMany<T>(p) {}

    QList<T> parse(Input &input) {
        QList<T> result;
        result.push_back(ParserMany<T>::p_->parse(input));
        result.append(ParserMany<T>::parse(input));

        return result;
    }
};

template<>
class ParserMany1<QChar> : public ParserMany<QChar> {
public:
    ParserMany1(Parser<QChar> *p);
    QString parse(Input &input);
};

template<typename T, typename TEnd>
class ParserManyTill : public Parser< QList<T> > {
protected:
    Parser<T> *p_;
    Parser<TEnd> *pend_;

public:
    ParserManyTill(Parser<T> *p, Parser<TEnd> *pend) : Parser< QList<T> >(), p_(p), pend_(pend) {}
    ~ParserManyTill(){
        delete p_;
        delete pend_;
    }

    QList<T> parse(Input &input) {
        QList<T> result;

        Q_FOREVER {
            try {
                pend_->parse(input);
                return result;
            }
            catch (const ParserException &) {
                result.push_back(p_->parse(input));
            }
        }
    }
};

template<typename T>
ParserMany<T> *Many(Parser<T> *p)
{ return new ParserMany<T>(p); }

ParserMany<QChar> *Many(Parser<QChar> *p);

template<typename T>
ParserMany1<T> *Many1(Parser<T> *p)
{ return new ParserMany1<T>(p); }

ParserMany1<QChar> *Many1(Parser<QChar> *p);

template<typename T, typename TEnd>
ParserManyTill<T, TEnd> *ManyTill(Parser<T> *p, Parser<TEnd> *pend)
{ return new ParserManyTill<T, TEnd>(p, pend); }

}
}

#endif // PARSERMANY

