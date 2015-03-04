#ifndef PARSERFAIL
#define PARSERFAIL

#include "parser.h"

namespace qparsec {
namespace prim {

class ParserFail : public Parser<void> {
protected:
    QString message_;

public:
    ParserFail(QString message) : message_(message) {}

    void parse(Input &input) {
        throw ParserException(input.index(), message_);
    }
};

ParserFail *Fail(QString message)
{ return new ParserFail(message); }

}
}

#endif // PARSERFAIL

