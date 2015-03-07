#ifndef QSCHEME_PARSER_EXPRESSION_PARSERDO_H
#define QSCHEME_PARSER_EXPRESSION_PARSERDO_H

#include "parser.h"
#include "ast/ast.h"

namespace qscheme {
namespace parser {
namespace expression {

class ParserDo : public qparsec::Parser<ast::SharedVal> {
protected:
    class ParserIterationSpec : public qparsec::Parser<QSharedPointer<ast::Do::IterationSpec>> {
    public:
        QSharedPointer<ast::Do::IterationSpec> parse(qparsec::Input &input);
    };
    Parser<QSharedPointer<ast::Do::IterationSpec>> *IterationSpec();

public:
    ast::SharedVal parse(qparsec::Input &input);
};

qparsec::Parser<ast::SharedVal> *Do();

}
}
}

#endif // QSCHEME_PARSER_EXPRESSION_PARSERDO_H
