#ifndef QSCHEME_PARSER_EXPRESSION_PARSERASSIGNMENT_H
#define QSCHEME_PARSER_EXPRESSION_PARSERASSIGNMENT_H

#include "parser.h"
#include "ast/ast.h"

namespace qscheme {
namespace parser {
namespace expression {

class ParserAssignment : public qparsec::Parser<ast::SharedVal> {
protected:
    Parser<QString> *Variable();

public:
    ast::SharedVal parse(qparsec::Input &input);
};

qparsec::Parser<ast::SharedVal> *Assignment();

}
}
}

#endif // QSCHEME_PARSER_EXPRESSION_PARSERASSIGNMENT_H
