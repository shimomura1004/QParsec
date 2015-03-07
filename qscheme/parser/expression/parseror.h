#ifndef QSCHEME_PARSER_EXPRESSION_PARSEROR_H
#define QSCHEME_PARSER_EXPRESSION_PARSEROR_H

#include "parser.h"
#include "ast/ast.h"

namespace qscheme {
namespace parser {
namespace expression {

qparsec::Parser<ast::SharedVal> *Or();

}
}
}

#endif // QSCHEME_PARSER_EXPRESSION_PARSEROR_H
