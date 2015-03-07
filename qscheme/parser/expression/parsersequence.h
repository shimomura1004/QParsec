#ifndef QSCHEME_PARSER_EXPRESSION_PARSERSEQUENCE_H
#define QSCHEME_PARSER_EXPRESSION_PARSERSEQUENCE_H

#include "parser.h"
#include "ast/ast.h"

namespace qscheme {
namespace parser {
namespace expression {

qparsec::Parser<QList<ast::SharedVal>> *Sequence();

}
}
}

#endif // QSCHEME_PARSER_EXPRESSION_PARSERSEQUENCE_H
