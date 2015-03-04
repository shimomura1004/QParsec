#include "parsersatisfy.h"

qparsec::Parser<QChar> *qparsec::character::Satisfy(bool (*judge)(QChar))
{ return new ParserSatisfy(judge); }

qparsec::character::ParserSatisfy::ParserSatisfy(bool (*judge)(QChar)) : Parser<QChar>(), judge_(judge) {}

QChar qparsec::character::ParserSatisfy::parse(qparsec::Input &input) {
    if (input.isEmpty())
        throw ParserException(input.index(), "Unexpected end of input");
    if (!judge_(input[0]))
        throw ParserException(input.index(), QStringLiteral("Not satisfied: %1").arg(input[0]));

    return input.consume(1)[0];
}
