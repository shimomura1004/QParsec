#ifndef QSCHEME_PARSER_EXPRESSION_PARSERCONDITION_H
#define QSCHEME_PARSER_EXPRESSION_PARSERCONDITION_H

#include "parser.h"
#include "ast/ast.h"

namespace qscheme {
namespace parser {
namespace expression {

class ParserCondition : public qparsec::Parser<ast::SharedVal> {
public:
    ast::SharedVal parse(qparsec::Input &input);
};

qparsec::Parser<ast::SharedVal> *Condition();

}
}
}

#endif // QSCHEME_PARSER_EXPRESSION_PARSERCONDITION_H
