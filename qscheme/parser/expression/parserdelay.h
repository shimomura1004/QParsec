#ifndef QSCHEME_PARSER_EXPRESSION_PARSERDELAY_H
#define QSCHEME_PARSER_EXPRESSION_PARSERDELAY_H

#include "parser.h"
#include "ast/ast.h"

namespace qscheme {
namespace parser {
namespace expression {

qparsec::Parser<ast::SharedVal> *Delay();

}
}
}

#endif // QSCHEME_PARSER_EXPRESSION_PARSERDELAY_H
