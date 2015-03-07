#ifndef QSCHEME_PARSER_EXPRESSION_PARSERQUOTATION_H
#define QSCHEME_PARSER_EXPRESSION_PARSERQUOTATION_H

#include "parser.h"
#include "ast/ast.h"

namespace qscheme {
namespace parser {
namespace expression {

class ParserQuotation : public qparsec::Parser<ast::SharedVal> {
    ast::SharedVal parse(qparsec::Input &input);
};

qparsec::Parser<ast::SharedVal> *Quotation();
}
}
}

#endif // QSCHEME_PARSER_EXPRESSION_PARSERQUOTATION_H
