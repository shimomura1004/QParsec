#ifndef QPARSEC_PARSER_DATUM_PARSERSTRING_H
#define QPARSEC_PARSER_DATUM_PARSERSTRING_H

#include "parser.h"
#include "ast/ast.h"

namespace qscheme {
namespace parser {
namespace datum {

qparsec::Parser<ast::SharedVal> *String();

}
}
}

#endif // QPARSEC_PARSER_DATUM_PARSERSTRING_H
