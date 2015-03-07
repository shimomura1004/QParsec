#ifndef QSCHEME_PARSER_EXPRESSION_PARSERPROCEDURECALL_H
#define QSCHEME_PARSER_EXPRESSION_PARSERPROCEDURECALL_H

#include "parser.h"
#include "ast/ast.h"

namespace qscheme {
namespace parser {
namespace expression {

qparsec::Parser<ast::SharedVal> *ProcedureCall();

}
}
}

#endif // QSCHEME_PARSER_EXPRESSION_PARSERPROCEDURECALL_H
