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

    ParserOneOf(QString chars, QChar *out = nullptr) : Parser(out), chars_(chars) {}

    QChar parse(Input &input) {
        if (input.isEmpty())
            throw ParserException(input.index(), "Unexpected end of input");
        if (!chars_.contains(input[0]))
            throw ParserException(input.index(), QStringLiteral("Expected one of '%1' but got '%2'").arg(chars_, QString(input[0])));

        QChar c = input.consume(1)[0];
        return setOut(c);
    }
};

struct ParserNoneOf : Parser<QChar> {
    QString chars_;

    ParserNoneOf(QString chars, QChar *out = nullptr) : Parser(out), chars_(chars) {}

    QChar parse(Input &input) {
        if (input.isEmpty())
            throw ParserException(input.index(), "Unexpected end of input");
        if (chars_.contains(input[0]))
            throw ParserException(input.index(), QStringLiteral("Expected none of '%1' but got '%2'").arg(chars_, QString(input[0])));

        QChar c = input.consume(1)[0];
        return setOut(c);
    }
};

struct ParserChar : Parser<QChar> {
    QChar c_;

    ParserChar(QChar c, QChar *out = nullptr) : Parser(out), c_(c) {}

    QChar parse(Input &input) {
        if (input.isEmpty())
            throw ParserException(input.index(), "Unexpected end of input");
        if (input[0] != c_)
            throw ParserException(input.index(), QStringLiteral("Expected '%1' but got '%2'").arg(QString(c_), QString(input[0])));

        input.consume(1);
        return setOut(c_);
    }
};

struct ParserStr : Parser<QString> {
    QString s_;

    ParserStr(QString s, QString *out = nullptr) : Parser(out), s_(s) {}

    QString parse(Input &input) {
        if (input.isEmpty())
            throw ParserException(0, "Empty input");
        if (!input.midRef(0).startsWith(s_))
            throw ParserException(input.index(), QStringLiteral("Expected '%1' but got '%2'").arg(s_, input.left(s_.length() - 1)));

        input.consume(s_.length());
        return setOut(s_);
    }
};

struct ParserAnyChar : Parser<QChar> {
    ParserAnyChar(QChar *out = nullptr) : Parser(out) {}

    QChar parse(Input &input) {
        if (input.isEmpty())
            throw ParserException(input.index(), "Unexpected end of input");

        QChar c = input.consume(1)[0];
        return setOut(c);
    }
};

ParserOneOf *OneOf(QString chars, QChar *out = nullptr)
{ return new ParserOneOf(chars, out); }

ParserNoneOf *NoneOf(QString chars, QChar *out = nullptr)
{ return new ParserNoneOf(chars, out); }

ParserChar *Char(QChar c, QChar *out = nullptr)
{ return new ParserChar(c, out); }

ParserStr *Str(QString s, QString *out = nullptr)
{ return new ParserStr(s, out); }

ParserAnyChar *AnyChar(QChar *out = nullptr)
{ return new ParserAnyChar(out); }

Parser<QChar> *Space(QChar *out = nullptr)
{ return OneOf(" \v\f\t\r\n", out); }

Parser<QString> *Spaces(QString *out = nullptr)
{ return combinators::Many(Space(), out); }

Parser<QChar> *Newline(QChar *out = nullptr)
{ return Char('\n', out); }

Parser<QChar> *Tab(QChar *out = nullptr)
{ return Char('\t', out); }

Parser<QChar> *Upper(QChar *out = nullptr)
{ return OneOf("ABCDEFGHIJKLMNOPQRSTUVWXYZ", out); }

Parser<QChar> *Lower(QChar *out = nullptr)
{ return OneOf("abcdefghijklmnopqrstuvwxyz", out); }

Parser<QChar> *Alphanum(QChar *out = nullptr)
{ return OneOf("abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789", out); }

Parser<QChar> *Letter(QChar *out = nullptr)
{ return OneOf("abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ", out); }

Parser<QChar> *Digit(QChar *out = nullptr)
{ return OneOf("0123456789", out); }

Parser<QChar> *Hexadigit(QChar *out = nullptr)
{ return OneOf("0123456789abcdefABCDEF", out); }

Parser<QChar> *Octdigit(QChar *out = nullptr)
{ return OneOf("01234567", out); }

struct ParserSatisfy : Parser<QChar> {
    bool(*judge_)(QChar);

    ParserSatisfy(bool(*judge)(QChar), QChar *out) : Parser<QChar>(out), judge_(judge) {}

    QChar parse(Input &input) {
        if (input.isEmpty())
            throw ParserException(input.index(), "Unexpected end of input");
        if (!judge_(input[0]))
            throw ParserException(input.index(), QStringLiteral("Not satisfied: %1").arg(input[0]));

        QChar c = input.consume(1)[0];
        return setOut(c);
    }
};

Parser<QChar> *Satisfy(bool(*judge)(QChar), QChar *out = nullptr)
{ return new ParserSatisfy(judge, out); }

}
}

#endif // QPARSECCHAR_H
