#ifndef QSCHEME_PARSER_EXPRESSION_PARSERLET_H
#define QSCHEME_PARSER_EXPRESSION_PARSERLET_H

#include "parser.h"
#include "ast/ast.h"

namespace qscheme {
namespace parser {
namespace expression {

class ParserLet : public qparsec::Parser<ast::SharedVal> {
public:
    ast::SharedVal parse(qparsec::Input &input);
};

qparsec::Parser<ast::SharedVal> *Let();

}
}
}

#endif // QSCHEME_PARSER_EXPRESSION_PARSERLET_H
