#include <QDebug>

#include "QParsec.h"
#include "QParsecCombinator.h"
#include "QParsecChar.h"

struct ParserStruct : Parser<void> {
  void parse(Input &input) {
      S(SkipMany(Space()))->parse(input);
      S(Str("struct"))->parse(input);
      S(SkipMany(Space()))->parse(input);
      S(Char('{'));
      S(Char('}'));
      S(SkipMany(Space()))->parse(input);
  }
};

int main() {
    {
    // try example
    {
        Input input("aabcd");
        try {
            auto result = S(Choice({ Try(Seq(Char('a'), Char('b'))),
                                     Seq(Char('a'), Char('a'))
                                   }))->parse(input);
            qDebug() << "result:" << result << input.value << input.stacks_;
        }
        catch (const ParserException &e) {
            qDebug() << "ParseError at" << e.index << ":" << e.reason;
        }
    }
#if 0
    // basic example
    {
        Input input("vector x = (12321,35)");

        try {
            QChar var;

            S(Choice({Str("int"), Str("vector")}))->parse(input);
            S(Seq(SkipMany1(Space()), AnyChar(&var)))->parse(input);
            S(Seq(SkipMany1(Space()), Char('=')))->parse(input);
            S(SkipMany1(Space()))->parse(input);

            S(Char('('))->parse(input);
            QString test;
            //auto tuple = S(SepBy(Many1(Digit(), &test), Char(',')))->parse(input);
            auto tuple = S(SepBy(Many(Digit(), &test), Char(',')))->parse(input);
            S(Char(')'))->parse(input);

            qDebug() << "set" << var << "to" << tuple;
            qDebug() << test;
        }
        catch (const ParserException& e) {
            qDebug() << "ParseError at" << e.index << ":" << e.reason;
        }
        qDebug() << input.value;
    }
#endif

    return 0;
}
