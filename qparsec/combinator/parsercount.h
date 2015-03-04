#ifndef QPARSEC_COMBINATOR_PARSERCOUNT
#define QPARSEC_COMBINATOR_PARSERCOUNT

#include <QString>
#include <QList>
#include "parser.h"

namespace qparsec {
namespace combinator {

template<typename T>
class ParserCount : public Parser< QList<T> > {
protected:
    Parser<T> *p_;
    int n_;

public:
    ParserCount(Parser<T> *p, int n) : Parser< QList<T> >(), p_(p), n_(n) {}
    ~ParserCount() { delete p_; }

    QList<T> parse(Input &input) {
        QList<T> result;

        for(int i = 0; i < n_; i++) {
            result.push_back(p_->parse(input));
        }
        return result;
    }
};

template<>
class ParserCount<QChar> : public Parser<QString> {
protected:
    Parser<QChar> *p_;
    int n_;

public:
    ParserCount(Parser<QChar> *p, int n);
    ~ParserCount();

    QString parse(Input &input);
};

template<typename T>
ParserCount<T> *Count(Parser<T> *p, int n)
{ return new ParserCount<T>(p, n); }
template<>
ParserCount<QChar> *Count(Parser<QChar> *p, int n);

}
}

#endif // QPARSEC_COMBINATOR_PARSERCOUNT

