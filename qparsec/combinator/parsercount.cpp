#include "parsercount.h"

namespace qparsec {
namespace combinator {

ParserCount<QChar>::ParserCount(Parser<QChar> *p, int n) : Parser<QString>(), p_(p), n_(n) {}

ParserCount<QChar>::~ParserCount() { delete p_; }

QString ParserCount<QChar>::parse(Input &input) {
    QString result;

    for(int i = 0; i < n_; i++) {
        result.push_back(p_->parse(input));
    }
    return result;
}

ParserCount<QChar> *Count(Parser<QChar> *p, int n)
{ return new ParserCount<QChar>(p, n); }

}
}
