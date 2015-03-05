#ifndef QSCHEME_PARSER_DATUM_PARSERCOMPOUNDDATUM_H
#define QSCHEME_PARSER_DATUM_PARSERCOMPOUNDDATUM_H

#include "parser.h"
#include "ast/ast.h"

namespace qscheme {
namespace parser {
namespace datum {

qparsec::Parser<ast::SharedVal> *CompoundDatum();

}
}
}

#endif // QSCHEME_PARSER_DATUM_PARSERCOMPOUNDDATUM_H
