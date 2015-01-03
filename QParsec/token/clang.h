#ifndef CLANG_H
#define CLANG_H

#include <QParsecChar.h>
#include <QParsecCombinator.h>
#include <token/scheme.h>

namespace qparsec {
namespace tokens {
namespace clang {

using namespace qparsec::chars;
using namespace qparsec::combinators;

Parser<int> *Decimal(int *out = nullptr) {
    return Apply<QString, int>(Lexeme(Many1(Digit())), [](QString s){return s.toInt();}, out);
}

Parser<int> *Hexadecimal(int *out = nullptr) {
    return Apply<QString, int>(
                Lexeme(Right(Seq(Char('0'), OneOf("xX")),
                             Many1(OneOf("0123456789abcdefABCDEF")))),
                [](QString s){return s.toInt(0, 16);},
                out);
}

Parser<int> *Octal(int *out = nullptr) {
    return Apply<QString, int>(
                Lexeme(Right(Seq(Char('0'), OneOf("oO")),
                             Many1(OneOf("01234567")))),
                [](QString s){return s.toInt(0, 8);},
                out);
}

Parser<int> *Natural(int *out = nullptr) {
    return Choice({Try(Hexadecimal()), Try(Octal()), Try(Decimal())}, out);
}

struct ParserInteger : Parser<int> {
    ParserInteger(int *out) : Parser<int>(out) {}

    int parse(Input &input) {
        auto sign = Choice({ Char('-'),
                             Option(Char('+'), QChar('+'))
                           })->parse(input);
        auto num = Natural()->parse(input);

        int result = (sign == '+' ? 1 : -1) * num;
        return setOut(result);
    }
};
ParserInteger *Integer(int *out = nullptr) { return new ParserInteger(out); }

struct ParserDouble : Parser<double> {
    ParserDouble(double *out) : Parser<double>(out) {}

    double parse(Input &input) {
        auto sign_b = Choice({ Char('-'),
                             Option(Char('+'), QChar('+'))
                           })->parse(input);
        auto num_b = Option(Natural(), 0)->parse(input);

        try
        {
            Char('.')->parse(input);
        }
        catch (const ParserException &){
            OneOf("eE")->parse(input);
            auto sign_e = Choice({ Char('-'),
                                   Option(Char('+'), QChar('+'))
                                 })->parse(input);
            auto num_e = Natural()->parse(input);
            return QStringLiteral("%1%2e%3%4").arg(sign_b,
                                                   QString::number(num_b),
                                                   sign_e,
                                                   QString::number(num_e)).toDouble();
        }
        auto num_e = Decimal()->parse(input);

        double result = QStringLiteral("%1%2.%3").arg(sign_b,
                                                      QString::number(num_b),
                                                      QString::number(num_e)).toDouble();
        return setOut(result);
    }
};
ParserDouble *Double(double *out = nullptr) { return new ParserDouble(out); }

}
}
}

#endif // CLANG_H
