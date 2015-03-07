#ifndef QSCHEME_PARSER_EXPRESSION_PARSERSELFEVALUATING_H
#define QSCHEME_PARSER_EXPRESSION_PARSERSELFEVALUATING_H

#include "parser.h"
#include "ast/ast.h"

namespace qscheme {
namespace parser {
namespace expression {

qparsec::Parser<ast::SharedVal> *SelfEvaluating();

}
}
}

#endif // QSCHEME_PARSER_EXPRESSION_PARSERSELFEVALUATING_H
