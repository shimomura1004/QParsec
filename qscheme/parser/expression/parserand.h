#ifndef QSCHEME_PARSER_EXPRESSION_PARSERAND_H
#define QSCHEME_PARSER_EXPRESSION_PARSERAND_H

#include "parser.h"
#include "ast/ast.h"

namespace qscheme {
namespace parser {
namespace expression {

qparsec::Parser<ast::SharedVal> *And();

}
}
}

#endif // QSCHEME_PARSER_EXPRESSION_PARSERAND_H
