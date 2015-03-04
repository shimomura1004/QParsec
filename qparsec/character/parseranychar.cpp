#include "parseranychar.h"

qparsec::character::ParserAnyChar::ParserAnyChar() : Parser() {}

QChar qparsec::character::ParserAnyChar::parse(qparsec::Input &input) {
    if (input.isEmpty())
        throw ParserException(input.index(), "Unexpected end of input");

    return input.consume(1)[0];
}
