#include <QCoreApplication>
#include <QParsec.h>
#include <QParsecChar.h>
#include <QParsecCombinator.h>
#include <QParsecToken.h>

#include <ast/AST.h>

#include <QDebug>

using namespace QParsec;

struct ParserPlus : Parser<int(*)(int,int)> {
    int (*parse(Input &input))(int, int) {
        Symbol("+")->parse(input);
        return [](int x, int y){ return x + y; };
    }
};

Parser<int> *Term() {
    Parser<int>*(*self)() = [](){return Term();};
    return  Lexeme( Parens( Chainl( Choice({ Lexeme(Decimal()), Lazy(self) }),
                                    new ParserPlus(),
                                    0)
                           ));
}

int main(int argc, char *argv[])
{
    Q_UNUSED(argc);
    Q_UNUSED(argv);

    try {
        Input input("(12 + 34 + (56 + 78) + 10 + (20))");
        auto sum = S(Term())->parse(input);
        qDebug() << sum;
    }
    catch (const ParserException &e) {
        qDebug() << e.index << e.reason;
    }

    return 0;
}
