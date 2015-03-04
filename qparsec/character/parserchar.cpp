#include "parserchar.h"

qparsec::character::ParserChar::ParserChar(QChar c) : Parser(), c_(c) {}

QChar qparsec::character::ParserChar::parse(qparsec::Input &input) {
    if (input.isEmpty())
        throw ParserException(input.index(), "Unexpected end of input");
    if (input[0] != c_)
        throw ParserException(input.index(), QStringLiteral("Expected '%1' but got '%2'").arg(QString(c_), QString(input[0])));

    input.consume(1);
    return c_;
}

qparsec::character::ParserChar *qparsec::character::Char(QChar c)
{ return new ParserChar(c); }

qparsec::Parser<QChar> *qparsec::character::Upper()
{ return OneOf("ABCDEFGHIJKLMNOPQRSTUVWXYZ"); }

qparsec::Parser<QChar> *qparsec::character::Lower()
{ return OneOf("abcdefghijklmnopqrstuvwxyz"); }

qparsec::Parser<QChar> *qparsec::character::Alphanum()
{ return OneOf("abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789"); }

qparsec::Parser<QChar> *qparsec::character::Letter()
{ return OneOf("abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ"); }

qparsec::Parser<QChar> *qparsec::character::Digit()
{ return OneOf("0123456789"); }

qparsec::Parser<QChar> *qparsec::character::Hexadigit()
{ return OneOf("0123456789abcdefABCDEF"); }

qparsec::Parser<QChar> *qparsec::character::Octdigit()
{ return OneOf("01234567"); }
