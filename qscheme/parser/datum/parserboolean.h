#ifndef QPARSEC_PARSER_DATUM_PARSERBOOLEAN_H
#define QPARSEC_PARSER_DATUM_PARSERBOOLEAN_H

#include "parser.h"
#include "ast/ast.h"

namespace qscheme {
namespace parser {
namespace datum {

qparsec::Parser<ast::SharedVal> *Boolean();

}
}
}

#endif // QPARSEC_PARSER_DATUM_PARSERBOOLEAN_H
