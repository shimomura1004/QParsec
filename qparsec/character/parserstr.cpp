#include "parserstr.h"

qparsec::character::ParserStr::ParserStr(QString s) : Parser(), s_(s) {}

QString qparsec::character::ParserStr::parse(qparsec::Input &input) {
    if (input.isEmpty())
        throw ParserException(0, "Empty input");
    if (!input.midRef(0).startsWith(s_))
        throw ParserException(input.index(), QStringLiteral("Expected '%1' but got '%2'").arg(s_, input.left(s_.length() - 1)));

    input.consume(s_.length());
    return s_;
}

qparsec::character::ParserStr *qparsec::character::Str(QString s)
{ return new ParserStr(s); }
