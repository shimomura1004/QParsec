#ifndef QPARSEC_PARSER_DATUM_PARSERSIMPLEDATUM_H
#define QPARSEC_PARSER_DATUM_PARSERSIMPLEDATUM_H

#include "parser.h"
#include "ast/ast.h"

namespace qscheme {
namespace parser {
namespace datum {

qparsec::Parser<ast::SharedVal> *SimpleDatum();

}
}
}
#endif // QPARSEC_PARSER_DATUM_PARSERSIMPLEDATUM_H
