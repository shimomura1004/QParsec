#ifndef QSCHEME_PARSER_EXPRESSION_PARSERBINDINGSPEC_H
#define QSCHEME_PARSER_EXPRESSION_PARSERBINDINGSPEC_H

#include "parser.h"
#include "ast/ast.h"

namespace qscheme {
namespace parser {
namespace expression {

qparsec::Parser<QPair<QString, ast::SharedVal>> *BindingSpec();

}
}
}

#endif // QSCHEME_PARSER_EXPRESSION_PARSERBINDINGSPEC_H
