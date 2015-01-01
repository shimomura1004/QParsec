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

Parser<QSharedPointer<AST::LispVal>> *LVal();
// we can't cast QSharedPointer<LispInt> to QSharedPointer<LispVal>
#define upcastToLispVal(p) reinterpret_cast<Parser<QSharedPointer<AST::LispVal>>*>(p)

Parser<QSharedPointer<AST::LispInt>> *LInt() {
    QSharedPointer<AST::LispInt>(*f)(int) = [](int n){ return AST::LispInt::create(n); };
    return Apply(Natural(), f);
}

struct ParserLLambda : Parser<QSharedPointer<AST::LispLambda>> {
    QSharedPointer<AST::LispLambda> parse(Input &input) {
        // (lambda (x y) (+ x y))
        Char('(')->parse(input);
        Symbol("lambda")->parse(input);
        auto vars = Parens(SepBy(Many1(OneOf("abcdefghijklmnopqrstuvwxyz")), SkipMany1(Space())))->parse(input);
        WhiteSpace()->parse(input);
        auto body = LVal()->parse(input);
        Char(')')->parse(input);
        return AST::LispLambda::create(vars, body, AST::Env());
    }
};
Parser<QSharedPointer<AST::LispLambda>> *LLambda() { return new ParserLLambda(); }

struct ParserLVal : Parser<QSharedPointer<AST::LispVal>> {
    QSharedPointer<AST::LispVal> parse(Input &input) {
        QSharedPointer<AST::LispVal> val =
                Choice({ upcastToLispVal(LInt()),
                         upcastToLispVal(LLambda()),
                         upcastToLispVal(Parens(new ParserLVal()))
                       })->parse(input);
        return val;
    }
};
Parser<QSharedPointer<AST::LispVal>> *LVal() { return new ParserLVal(); }

int main(int argc, char *argv[])
{
    Q_UNUSED(argc);
    Q_UNUSED(argv);

    {
        Input input("(lambda (x) 123)");
        qDebug() << LVal()->parse(input)->toString();
    }

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
