#include <QDebug>

#include "QParsec.h"
#include "QParsecCombinator.h"
#include "QParsecChar.h"

#include <QSharedPointer>

template<typename T>
QSharedPointer< Parser<T> > S(Parser<T> *p)
{ return QSharedPointer< Parser<T> >(p);}

int main() {
    {
        Input input("vector x = (12321,35)");

        try {
            S(Choice({Str("int"), Str("vector")}))->parse(input);
            S(Many1(Space()))->parse(input);
            S(Char('x'))->parse(input);
            S(Many1(Space()))->parse(input);
            S(Char('='))->parse(input);
            S(Many1(Space()))->parse(input);

            S(Char('('))->parse(input);
            auto zeros = S(SepBy(Many1(Digit()), Char(',')))->parse(input);
            S(Char(')'))->parse(input);
            qDebug() << "zeros:" << zeros;

            qDebug() << "ParseResult:" << left << right;
        }
        catch (const ParserException& e) {
            qDebug() << "ParseError at" << e.index << ":" << e.reason;
            qDebug() << input.value;
        }
    }

    return 0;
}
