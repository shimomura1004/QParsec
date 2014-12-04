#ifndef QPARSECCHAR_H
#define QPARSECCHAR_H

#include "QParsec.h"
#include "QParsecCombinator.h"

#include <QChar>
#include <QString>

struct ParserChar : Parser<QChar> {
    QChar c_;

    ParserChar(char c) : c_(c) {}
    ParserChar(QChar c) : c_(c) {}

    QChar parse(Input &input) {
        if (input.value.isEmpty())
            throw ParserException(0, "Empty input");
        if (input.value[0] != c_)
            throw ParserException(input.index, QStringLiteral("Expected '%1' but got '%2'").arg(QString(c_), QString(input.value[0])));

        input.value.remove(0, 1);
        input.index += 1;
        return c_;
    }
};

struct ParserStr : Parser<QString> {
    QString s_;

    ParserStr(char *s) : s_(s) {}
    ParserStr(QString s) : s_(s) {}

    QString parse(Input &input) {
        if (input.value.isEmpty())
            throw ParserException(0, "Empty input");
        if (!input.value.startsWith(s_))
            throw ParserException(input.index, QStringLiteral("Expected '%1' but got '%2'").arg(s_, input.value.mid(0, s_.length())));

        input.value.remove(0, s_.length());
        input.index += s_.length();
        return s_;
    }
};

struct ParserDigit : Parser<QChar> {
    static ParserChoice<QChar> p_;

    QChar parse(Input &input) {
        return p_.parse(input);
    }
};
ParserChoice<QChar>
ParserDigit::p_({ new ParserChar('0'),
                  new ParserChar('1'),
                  new ParserChar('2'),
                  new ParserChar('3'),
                  new ParserChar('4'),
                  new ParserChar('5'),
                  new ParserChar('6'),
                  new ParserChar('7'),
                  new ParserChar('8'),
                  new ParserChar('9')
                });

struct ParserSpace : Parser<QChar> {
    static ParserChoice<QChar> p_;

    QChar parse(Input &input) {
        return p_.parse(input);
    }
};
ParserChoice<QChar>
ParserSpace::p_({ new ParserChar(' '),
                  new ParserChar('\v'),
                  new ParserChar('\f'),
                  new ParserChar('\t'),
                  new ParserChar('\r'),
                  new ParserChar('\n')
                });

ParserChar *Char(char c) { return new ParserChar(c); }
ParserStr *Str(QString s) { return new ParserStr(s); }
ParserDigit *Digit() { return new ParserDigit(); }
ParserSpace *Space() { return new ParserSpace(); }

#endif // QPARSECCHAR_H
