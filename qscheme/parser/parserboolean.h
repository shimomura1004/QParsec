#ifndef PARSERBOOLEAN_H
#define PARSERBOOLEAN_H

#include "parser.h"
#include "ast/ast.h"

namespace qscheme {
namespace parser {

qparsec::Parser<ast::SharedVal> *Boolean();

}
}

#endif // PARSERBOOLEAN_H
