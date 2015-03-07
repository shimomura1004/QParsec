#ifndef QSCHEME_PARSER_EXPRESSION_PARSERBODY_H
#define QSCHEME_PARSER_EXPRESSION_PARSERBODY_H

#include "parser.h"
#include "ast/ast.h"

namespace qscheme {
namespace parser {
namespace expression {

qparsec::Parser<QList<ast::SharedVal>> *Body();

}
}
}

#endif // QSCHEME_PARSER_EXPRESSION_PARSERBODY_H
