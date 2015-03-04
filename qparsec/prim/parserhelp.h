#ifndef QPARSEC_PRIM_PARSERHELP
#define QPARSEC_PRIM_PARSERHELP

#include "parser.h"
#include "parserexception.h"

namespace qparsec {
namespace prim {

template<typename T>
class ParserHelp : public Parser<T> {
protected:
    Parser<T> *p_;
    QString message_;

public:
    ParserHelp(Parser<T> *p, const QString &message) : p_(p), message_(message) {}
    virtual ~ParserHelp() {delete p_;}

    T parse(Input &input) {
        try {
            return p_->parse(input);
        }
        catch (const ParserException &e) {
            ParserException e2(e.index(), message_);
            throw e2;
        }
    }
};

template<typename T>
ParserHelp<T> *Help(Parser<T> *p, const QString &message)
{ return new ParserHelp<T>(p, message); }

}
}

#endif // QPARSEC_PRIM_PARSERHELP

