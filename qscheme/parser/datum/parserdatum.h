#ifndef QSCHEME_PARSER_DATUM_PARSERDATUM_H
#define QSCHEME_PARSER_DATUM_PARSERDATUM_H

#include "parser.h"
#include "ast/ast.h"

namespace qscheme {
namespace parser {
namespace datum {

class ParserDatum : public qparsec::Parser<ast::SharedVal> {
      ast::SharedVal parse(qparsec::Input &input);
};
qparsec::Parser<ast::SharedVal> *Datum();

}
}
}

#endif // QSCHEME_PARSER_DATUM_PARSERDATUM_H
