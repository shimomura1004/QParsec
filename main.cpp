#include <QCoreApplication>
#include <QDebug>

#include "QParsec.h"
#include "QParsecCombinator.h"
#include "QParsecChar.h"

int main() {
    {
        Input input("(10,01)");

        // tuple parser
        try {
            Char('(').parse(input);
            auto num1 = QList< Parser<QChar>* >() << new Char('0') << new Char('1');
            auto left = Many1<QChar>(new Choice<QChar>(num1)).parse(input);
            Char(',').parse(input);
            auto num2 = QList< Parser<QChar>* >() << new Char('0') << new Char('1');
            auto right = Many1<QChar>(new Choice<QChar>(num2)).parse(input);
            Char(')').parse(input);

            qDebug() << "ParseResult:" << left << right;
        }
        catch (const ParserException& e) {
            qDebug() << "ParseError at" << e.index << ":" << e.reason;
        }

        qDebug() << "Input:" << input.value;
    }

    {
        Input input("int x = (10101,01)");

        try {
            Str(QStringLiteral("int")).parse(input);
            Many<QChar>(new Char(' ')).parse(input);
            Char('x').parse(input);
            Many<QChar>(new Char(' ')).parse(input);
            Char('=').parse(input);
            Many<QChar>(new Char(' ')).parse(input);

            Char('(').parse(input);
            auto num1 = QList< Parser<QChar>* >() << new Char('0') << new Char('1');
            auto left = Many1<QChar>(new Choice<QChar>(num1)).parse(input);
            Char(',').parse(input);
            auto num2 = QList< Parser<QChar>* >() << new Char('0') << new Char('1');
            auto right = Many1<QChar>(new Choice<QChar>(num2)).parse(input);
            Char(')').parse(input);

            qDebug() << "ParseResult:" << left << right;
        }
        catch (const ParserException& e) {
            qDebug() << "ParseError at" << e.index << ":" << e.reason;
        }
    }

    return 0;
}
