#ifndef QSCHEME_PARSER_DATUM_PARSERLIST_H
#define QSCHEME_PARSER_DATUM_PARSERLIST_H

#include "parser.h"
#include "ast/ast.h"

namespace qscheme {
namespace parser {
namespace datum {

class ParserList : public qparsec::Parser<ast::SharedVal> {
protected:
    struct ParserAbbriviation : Parser<ast::SharedVal> {
        ast::SharedVal parse(qparsec::Input &input);
    };
    Parser<ast::SharedVal> *Abbriviation();

public:
    ast::SharedVal parse(qparsec::Input &input);
};

qparsec::Parser<ast::SharedVal> *List();

}
}
}

#endif // QSCHEME_PARSER_DATUM_PARSERLIST_H
