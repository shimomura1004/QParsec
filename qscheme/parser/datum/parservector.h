#ifndef QSCHEME_PARSER_DATUM_PARSERVECTOR_H
#define QSCHEME_PARSER_DATUM_PARSERVECTOR_H

#include "parser.h"
#include "ast/ast.h"

namespace qscheme {
namespace parser {
namespace datum {

qparsec::Parser<ast::SharedVal> *Vector();

}
}
}

#endif // QSCHEME_PARSER_DATUM_PARSERVECTOR_H
