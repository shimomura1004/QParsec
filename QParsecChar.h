#ifndef QPARSECCHAR_H
#define QPARSECCHAR_H

#include "QParsec.h"
#include "QParsecCombinator.h"

#include <QChar>
#include <QString>

// todo: custom error message

struct ParserOneOf : Parser<QChar> {
    QString chars_;

    ParserOneOf(QString chars, QChar *out = nullptr) : Parser(out), chars_(chars) {}
    ParserOneOf(char* chars, QChar *out = nullptr) : Parser(out), chars_(chars) {}

    QChar parse(Input &input) {
        if (input.value.isEmpty())
            throw ParserException(input.index, "Unexpected end of input");
        if (!chars_.contains(input.value[0]))
            throw ParserException(input.index, QStringLiteral("Expected one of '%1' but got '%2'").arg(chars_, input.value.mid(0, 1)));
        QChar c = input.value[0];
        input.value.remove(0, 1);
        input.index += 1;

        if (out_)
            *out_ = c;
        return c;
    }
};

struct ParserNoneOf : Parser<QChar> {
    QString chars_;

    ParserNoneOf(QString chars, QChar *out = nullptr) : Parser(out), chars_(chars) {}
    ParserNoneOf(char* chars, QChar *out = nullptr) : Parser(out), chars_(chars) {}

    QChar parse(Input &input) {
        if (input.value.isEmpty())
            throw ParserException(input.index, "Unexpected end of input");
        if (chars_.contains(input.value[0]))
            throw ParserException(input.index, QStringLiteral("Expected none of '%1' but got '%2'").arg(chars_, input.value.mid(0, 1)));
        QChar c = input.value[0];
        input.value.remove(0, 1);
        input.index += 1;

        if (out_)
            *out_ = c;
        return c;
    }
};

struct ParserChar : Parser<QChar> {
    QChar c_;

    ParserChar(QChar c, QChar *out = nullptr) : Parser(out), c_(c) {}

    QChar parse(Input &input) {
        if (input.value.isEmpty())
            throw ParserException(input.index, "Unexpected end of input");
        if (input.value[0] != c_)
            throw ParserException(input.index, QStringLiteral("Expected '%1' but got '%2'").arg(QString(c_), input.value.mid(0, 1)));

        input.value.remove(0, 1);
        input.index += 1;

        if (out_)
            *out_ = c_;
        return c_;
    }
};

struct ParserStr : Parser<QString> {
    QString s_;

    ParserStr(QString s, QString *out = nullptr) : Parser(out), s_(s) {}

    QString parse(Input &input) {
        if (input.value.isEmpty())
            throw ParserException(0, "Empty input");
        if (!input.value.startsWith(s_))
            throw ParserException(input.index, QStringLiteral("Expected '%1' but got '%2'").arg(s_, input.value.mid(0, s_.length())));

        input.value.remove(0, s_.length());
        input.index += s_.length();

        if (out_)
            *out_ = s_;
        return s_;
    }
};

struct ParserAnyChar : Parser<QChar> {
    ParserAnyChar(QChar *out = nullptr) : Parser(out) {}

    QChar parse(Input &input) {
        if (input.value.isEmpty())
            throw ParserException(input.index, "Unexpected end of input");
        QChar c = input.value[0];
        input.value.remove(0, 1);
        input.index += 1;

        if (out_)
            *out_ = c;
        return c;
    }
};

struct ParserDigit : Parser<QChar> {
    static ParserOneOf p_;

    ParserDigit(QChar *out = nullptr) : Parser(out) {}

    QChar parse(Input &input) {
        QChar c = p_.parse(input);
        if (out_)
            *out_ = c;
        return c;
    }
};
ParserOneOf
ParserDigit::p_("0123456789");

struct ParserSpace : Parser<QChar> {
    static ParserOneOf p_;

    ParserSpace(QChar *out = nullptr) : Parser(out) {}

    QChar parse(Input &input) {
        QChar c =  p_.parse(input);
        if (out_)
            *out_ = c;
        return c;
    }
};
ParserOneOf
ParserSpace::p_(" \v\f\t\r\n");


ParserOneOf *OneOf(QString chars, QChar *out = nullptr)
{ return new ParserOneOf(chars, out); }
ParserOneOf *OneOf(char* chars, QChar *out = nullptr)
{ return new ParserOneOf(chars, out); }

ParserNoneOf *NoneOf(QString chars, QChar *out = nullptr)
{ return new ParserNoneOf(chars, out); }
ParserNoneOf *NoneOf(char* chars, QChar *out = nullptr)
{ return new ParserNoneOf(chars, out); }

ParserChar *Char(QChar c, QChar *out = nullptr)
{ return new ParserChar(c, out); }

ParserStr *Str(QString s, QString *out = nullptr)
{ return new ParserStr(s, out); }

ParserAnyChar *AnyChar(QChar *out = nullptr)
{ return new ParserAnyChar(out); }

ParserDigit *Digit(QChar *out = nullptr)
{ return new ParserDigit(out); }

ParserSpace *Space(QChar *out = nullptr)
{ return new ParserSpace(out); }

#endif // QPARSECCHAR_H
