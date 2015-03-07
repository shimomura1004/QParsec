#ifndef QSCHEME_PARSER_EXPRESSION_PARSERLAMBDA_H
#define QSCHEME_PARSER_EXPRESSION_PARSERLAMBDA_H

#include "parser.h"
#include "ast/ast.h"

namespace qscheme {
namespace parser {
namespace expression {

class ParserLambda : public qparsec::Parser<ast::SharedVal> {
protected:
    struct ParserFormals : Parser<QPair<QStringList, QString>> {
        QPair<QStringList, QString> parse(qparsec::Input &input);
    };
    Parser<QPair<QStringList, QString>> *Formals();

public:
    ast::SharedVal parse(qparsec::Input &input);
};

qparsec::Parser<ast::SharedVal> *Lambda();

}
}
}

#endif // QSCHEME_PARSER_EXPRESSION_PARSERLAMBDA_H
