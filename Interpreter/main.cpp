#include <QCoreApplication>
#include <QParsec.h>
#include <QParsecChar.h>
#include <QParsecCombinator.h>
#include <QParsecToken.h>

#include <QDebug>

ParserApply<QString, int> *Num() {
    return Apply<QString, int>(Many1(Digit()), [](QString digit){ return digit.toInt(); });
}

struct ParserNum : Parser<int> {
    int parse(Input &input) {
        int result = Many1(Digit())->parse(input).toInt();
        return setOut(result);
    }
};

typedef int(*intoperator)(int, int);
struct ParserPlus : Parser<int(*)(int,int)> {
    int (*parse(Input &input))(int, int) {
        Symbol("+")->parse(input);
        return [](int x, int y){ return x + y; };
    }
};

struct ParserTerm : Parser<int> {
    int parse(Input &input) {
        auto sum = Lexeme( Between( Chainl( Choice({ Lexeme(Num()), new ParserTerm() }),
                                            new ParserPlus(),
                                            0),
                                    Char('('),
                                    Char(')')))->parse(input);
        return setOut(sum);
    }
};

int main(int argc, char *argv[])
{
    Q_UNUSED(argc);
    Q_UNUSED(argv);

    try {
        Input input("(12 + 34 + (56 + 78) + 10 + (20))");
        auto sum = S(new ParserTerm())->parse(input);
        qDebug() << sum;
    }
    catch (const ParserException &e) {
        qDebug() << e.index << e.reason;
    }

    return 0;
}
