#include "parsereof.h"

void qparsec::combinator::ParserEof::parse(qparsec::Input &input) {
    if (!input.isEmpty())
        throw ParserException(input.index(), "Isn't at the end of input");
}

qparsec::combinator::ParserEof *qparsec::combinator::Eof()
{ return new ParserEof(); }
