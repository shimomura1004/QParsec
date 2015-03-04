#include "parserfail.h"
#include "parserexception.h"

namespace qparsec {
namespace prim {

ParserFail::ParserFail(QString message) : message_(message) {}

void ParserFail::parse(qparsec::Input &input) {
    throw ParserException(input.index(), message_);
}

ParserFail *Fail(QString message)
{ return new ParserFail(message); }

}
}
