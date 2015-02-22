#include <QCoreApplication>
#include <iostream>

#include <QParsec.h>
#include <QParsecChar.h>
#include <QParsecCombinator.h>
#include <token/scheme.h>

#include <lisp/ast.h>
#include <lisp/parser.h>

#include <QDebug>

using namespace qparsec;

// todo: parser should return wheather it consumes input
// todo: fails parser should return expected characters
// propose: return ParserResult<T> instead of using exception

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
            tokens::WhiteSpace()->parse(input);
            //auto result = Left(parser::Val(), tokens::Eof())->parse(input);
            auto result = S(Left(parser::expression::Expression(), tokens::Eof()))->parse(input);
            std::cout << result->toString().toLatin1().constData() << std::endl;
        }
        catch (const ParserException &e) {
            std::cout << "ParseError at " << e.index << std::endl;

            std::cout << line.mid(e.index - 10, 32).toLatin1().constData() <<std::endl;
            for(int i = 0; i < qMin(e.index, 10); i++)
                std::cout << " ";
            std::cout << "^ " << e.reason.toLatin1().constData() << std::endl;
        }
    }

    return 0;
}
