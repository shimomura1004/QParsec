#ifndef QPARSEC_PARSER_DATUM_PARSERVARIABLE_H
#define QPARSEC_PARSER_DATUM_PARSERVARIABLE_H

#include "parser.h"
#include "ast/ast.h"

namespace qscheme {
namespace parser {
namespace datum {

qparsec::Parser<ast::SharedVal> *Variable();

}
}
}

#endif // QPARSEC_PARSER_DATUM_PARSERVARIABLE_H
