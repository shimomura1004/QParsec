#include <iostream>
#include <QTextStream>

#include "parser.h"
#include "parserexception.h"
#include "prim/prim.h"
#include "token/token.h"
#include "parser/expression/expression.h"

using namespace qparsec;
using namespace prim;
using namespace token;
using namespace qscheme::parser;

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
            token::WhiteSpace()->parse(input);
            auto result = (Left(expression::Expression(), Eof()))->parse(input);
            std::cout << result->toString().toLatin1().constData() << std::endl;
        }
        catch (const ParserException &e) {
            std::cout << "ParseError at " << e.index() << std::endl;

            std::cout << line.mid(e.index() - 10, 32).toLatin1().constData() <<std::endl;
            for(int i = 0; i < qMin(e.index(), 10); i++)
                std::cout << " ";
            std::cout << "^ " << e.reason().toLatin1().constData() << std::endl;
        }
    }

    return 0;
}
