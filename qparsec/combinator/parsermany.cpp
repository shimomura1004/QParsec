#include "parsermany.h"

namespace qparsec {
namespace combinator {

ParserMany<QChar>::ParserMany(Parser<QChar> *p) : Parser(), p_(p) {}

ParserMany<QChar>::~ParserMany() {delete p_;}

QString ParserMany<QChar>::parse(Input &input) {
    QString result;
    try {
        Q_FOREVER {
            result += p_->parse(input);
        }
    }
    catch (const ParserException &) {
        return result;
    }
}

ParserMany<QChar> *Many(Parser<QChar> *p)
{ return new ParserMany<QChar>(p); }

ParserMany1<QChar>::ParserMany1(Parser<QChar> *p) : ParserMany<QChar>(p) {}

QString ParserMany1<QChar>::parse(Input &input) {
    QString result;
    result += ParserMany<QChar>::p_->parse(input);
    result += ParserMany<QChar>::parse(input);

    return result;
}


ParserMany1<QChar> *Many1(Parser<QChar> *p)
{ return new ParserMany1<QChar>(p); }

}
}
