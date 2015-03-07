#ifndef QSCHEME_PARSER_EXPRESSION_PARSERBEGIN_H
#define QSCHEME_PARSER_EXPRESSION_PARSERBEGIN_H

#include "parser.h"
#include "ast/ast.h"

namespace qscheme {
namespace parser {
namespace expression {

qparsec::Parser<ast::SharedVal> *Begin();

}
}
}

#endif // QSCHEME_PARSER_EXPRESSION_PARSERBEGIN_H
