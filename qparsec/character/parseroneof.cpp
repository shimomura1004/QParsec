#include "parseroneof.h"

qparsec::character::ParserOneOf::ParserOneOf(QString chars) : Parser(), chars_(chars) {}

QChar qparsec::character::ParserOneOf::parse(qparsec::Input &input) {
    if (input.isEmpty())
        throw ParserException(input.index(), "Unexpected end of input");
    if (!chars_.contains(input[0]))
        throw ParserException(input.index(), QStringLiteral("Expected one of '%1' but got '%2'").arg(chars_, QString(input[0])));

    return input.consume(1)[0];
}

qparsec::character::ParserOneOf *qparsec::character::OneOf(QString chars)
{ return new ParserOneOf(chars); }
