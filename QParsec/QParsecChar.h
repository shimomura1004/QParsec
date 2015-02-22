#ifndef QPARSECCHAR_H
#define QPARSECCHAR_H

#include "QParsec.h"
#include "QParsecCombinator.h"

#include <QChar>
#include <QString>

namespace qparsec {
namespace chars {

struct ParserOneOf : Parser<QChar> {
    QString chars_;

    ParserOneOf(QString chars) : Parser(), chars_(chars) {}

    QChar parse(Input &input) {
        if (input.isEmpty())
            throw ParserException(input.index(), "Unexpected end of input");
        if (!chars_.contains(input[0]))
            throw ParserException(input.index(), QStringLiteral("Expected one of '%1' but got '%2'").arg(chars_, QString(input[0])));

        return input.consume(1)[0];
    }
};

struct ParserNoneOf : Parser<QChar> {
    QString chars_;

    ParserNoneOf(QString chars) : Parser(), chars_(chars) {}

    QChar parse(Input &input) {
        if (input.isEmpty())
            throw ParserException(input.index(), "Unexpected end of input");
        if (chars_.contains(input[0]))
            throw ParserException(input.index(), QStringLiteral("Expected none of '%1' but got '%2'").arg(chars_, QString(input[0])));

        return input.consume(1)[0];
    }
};

struct ParserChar : Parser<QChar> {
    QChar c_;

    ParserChar(QChar c) : Parser(), c_(c) {}

    QChar parse(Input &input) {
        if (input.isEmpty())
            throw ParserException(input.index(), "Unexpected end of input");
        if (input[0] != c_)
            throw ParserException(input.index(), QStringLiteral("Expected '%1' but got '%2'").arg(QString(c_), QString(input[0])));

        input.consume(1);
        return c_;
    }
};

struct ParserStr : Parser<QString> {
    QString s_;

    ParserStr(QString s) : Parser(), s_(s) {}

    QString parse(Input &input) {
        if (input.isEmpty())
            throw ParserException(0, "Empty input");
        if (!input.midRef(0).startsWith(s_))
            throw ParserException(input.index(), QStringLiteral("Expected '%1' but got '%2'").arg(s_, input.left(s_.length() - 1)));

        input.consume(s_.length());
        return s_;
    }
};

struct ParserAnyChar : Parser<QChar> {
    ParserAnyChar() : Parser() {}

    QChar parse(Input &input) {
        if (input.isEmpty())
            throw ParserException(input.index(), "Unexpected end of input");

        return input.consume(1)[0];
    }
};

ParserOneOf *OneOf(QString chars)
{ return new ParserOneOf(chars); }

ParserNoneOf *NoneOf(QString chars)
{ return new ParserNoneOf(chars); }

ParserChar *Char(QChar c)
{ return new ParserChar(c); }

ParserStr *Str(QString s)
{ return new ParserStr(s); }

ParserAnyChar *AnyChar()
{ return new ParserAnyChar(); }

Parser<QChar> *Space()
{ return OneOf(" \v\f\t\r\n"); }

Parser<QString> *Spaces()
{ return combinators::Many(Space()); }

Parser<QChar> *Newline()
{ return Char('\n'); }

Parser<QChar> *Tab()
{ return Char('\t'); }

Parser<QChar> *Upper()
{ return OneOf("ABCDEFGHIJKLMNOPQRSTUVWXYZ"); }

Parser<QChar> *Lower()
{ return OneOf("abcdefghijklmnopqrstuvwxyz"); }

Parser<QChar> *Alphanum()
{ return OneOf("abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789"); }

Parser<QChar> *Letter()
{ return OneOf("abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ"); }

Parser<QChar> *Digit()
{ return OneOf("0123456789"); }

Parser<QChar> *Hexadigit()
{ return OneOf("0123456789abcdefABCDEF"); }

Parser<QChar> *Octdigit()
{ return OneOf("01234567"); }

struct ParserSatisfy : Parser<QChar> {
    bool(*judge_)(QChar);

    ParserSatisfy(bool(*judge)(QChar)) : Parser<QChar>(), judge_(judge) {}

    QChar parse(Input &input) {
        if (input.isEmpty())
            throw ParserException(input.index(), "Unexpected end of input");
        if (!judge_(input[0]))
            throw ParserException(input.index(), QStringLiteral("Not satisfied: %1").arg(input[0]));

        return input.consume(1)[0];
    }
};

Parser<QChar> *Satisfy(bool(*judge)(QChar))
{ return new ParserSatisfy(judge); }

}
}

#endif // QPARSECCHAR_H
