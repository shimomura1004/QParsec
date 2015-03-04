#ifndef QPARSEC_PARSER_DATUM_PARSERCHARACTER_H
#define QPARSEC_PARSER_DATUM_PARSERCHARACTER_H

#include "parser.h"
#include "ast/ast.h"

namespace qscheme {
namespace parser {
namespace datum {

qparsec::Parser<ast::SharedVal> *Character();

}
}
}

#endif // QPARSEC_PARSER_DATUM_PARSERCHARACTER_H
