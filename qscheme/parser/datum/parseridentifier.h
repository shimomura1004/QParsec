#ifndef QPARSEC_PARSER_DATUM_PARSERIDENTIFIER_H
#define QPARSEC_PARSER_DATUM_PARSERIDENTIFIER_H

#include "parser.h"
#include "ast/ast.h"

namespace qscheme {
namespace parser {
namespace datum {

qparsec::Parser<ast::SharedVal> *Identifier();

}
}
}

#endif // QPARSEC_PARSER_DATUM_PARSERIDENTIFIER_H
