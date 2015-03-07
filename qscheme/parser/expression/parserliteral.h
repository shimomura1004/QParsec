#ifndef QSCHEME_PARSER_EXPRESSION_PARSERLITERAL_H
#define QSCHEME_PARSER_EXPRESSION_PARSERLITERAL_H

#include "parser.h"
#include "ast/ast.h"

namespace qscheme {
namespace parser {
namespace expression {

qparsec::Parser<ast::SharedVal> *Literal();

}
}
}

#endif // QSCHEME_PARSER_EXPRESSION_PARSERLITERAL_H
