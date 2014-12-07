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
        Input input("vector x = (12321,35)");

        try {
            S(Choice({Str("int"), Str("vector")}))->parse(input);
            S(SkipMany1(Space()))->parse(input);
            auto var = S(AnyChar())->parse(input);
            S(SkipMany1(Space()))->parse(input);
            S(Char('='))->parse(input);
            S(SkipMany1(Space()))->parse(input);

            S(Char('('))->parse(input);
            auto tuple = S(SepBy(Many1(Digit()), Char(',')))->parse(input);
            S(Char(')'))->parse(input);

            qDebug() << "set" << var << "to" << tuple;
        }
        catch (const ParserException& e) {
            qDebug() << "ParseError at" << e.index << ":" << e.reason;
        }
        qDebug() << input.value;
    }

    return 0;
}
