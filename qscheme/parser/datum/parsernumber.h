#ifndef QPARSEC_PARSER_DATUM_PARSERNUMBER_H
#define QPARSEC_PARSER_DATUM_PARSERNUMBER_H

#include "parser.h"
#include "ast/ast.h"

namespace qscheme {
namespace parser {
namespace datum {

qparsec::Parser<ast::SharedVal> *Number();

}
}
}

#endif // QPARSEC_PARSER_DATUM_PARSERNUMBER_H
