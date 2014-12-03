#ifndef QPARSECCHAR_H
#define QPARSECCHAR_H

#include "QParsec.h"

#include <QChar>
#include <QString>

struct Char : Parser<QChar> {
    QChar c_;

    Char(char c) : c_(c) {}
    Char(QChar c) : c_(c) {}

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

struct Str : Parser<QString> {
    QString s_;

    Str(char *s) : s_(s) {}
    Str(QString s) : s_(s) {}

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

#endif // QPARSECCHAR_H
