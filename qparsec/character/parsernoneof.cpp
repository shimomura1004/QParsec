#include "parsernoneof.h"

qparsec::character::ParserNoneOf::ParserNoneOf(QString chars) : Parser(), chars_(chars) {}

QChar qparsec::character::ParserNoneOf::parse(qparsec::Input &input) {
    if (input.isEmpty())
        throw ParserException(input.index(), "Unexpected end of input");
    if (chars_.contains(input[0]))
        throw ParserException(input.index(), QStringLiteral("Expected none of '%1' but got '%2'").arg(chars_, QString(input[0])));

    return input.consume(1)[0];
}

qparsec::character::ParserNoneOf *qparsec::character::NoneOf(QString chars)
{ return new ParserNoneOf(chars); }
