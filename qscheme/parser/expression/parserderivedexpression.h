#ifndef QSCHEME_PARSER_EXPRESSION_PARSERDERIVEDEXPRESSION_H
#define QSCHEME_PARSER_EXPRESSION_PARSERDERIVEDEXPRESSION_H

#include "parser.h"
#include "ast/ast.h"

namespace qscheme {
namespace parser {
namespace expression {

class ParserDerivedExpression : public qparsec::Parser<ast::SharedVal> {
public:
    ast::SharedVal parse(qparsec::Input &input);
};

qparsec::Parser<ast::SharedVal> *DerivedExpression();

}
}
}

#endif // QSCHEME_PARSER_EXPRESSION_PARSERDERIVEDEXPRESSION_H
