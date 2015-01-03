#include <QCoreApplication>
#include <iostream>

#include <QParsec.h>
#include <QParsecChar.h>
#include <QParsecCombinator.h>
#include <token/lisp.h>

#include <lisp/ast.h>
#include <lisp/parser.h>

#include <QDebug>

using namespace qparsec;
using namespace qparsec::tokens;

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

using namespace qparsec::tokens::lisp;

int main(int argc, char *argv[])
{
    Q_UNUSED(argc);
    Q_UNUSED(argv);

    QTextStream in(stdin, QIODevice::ReadOnly);
    Q_FOREVER {
        std::cout << "> ";
        std::cout.flush();

        QString line = in.readLine();
        if (line.isEmpty())
            break;

        Input input(line);

        try {
            WhiteSpace()->parse(input);
            qDebug() << lisp::parser::Val()->parse(input)->toString();
        }
        catch (const ParserException &e) {
            qDebug() << "ParseError at" << e.index;
            qDebug("%s", e.reason.toLatin1().constData());
        }
    }

    return 0;
}
