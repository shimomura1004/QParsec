#ifndef QSCHEME_PARSER_EXPRESSION_PARSERCOND_H
#define QSCHEME_PARSER_EXPRESSION_PARSERCOND_H

#include "parser.h"
#include "ast/ast.h"

namespace qscheme {
namespace parser {
namespace expression {

class ParserCond : public qparsec::Parser<ast::SharedVal> {
protected:
    struct ParserCondClause : Parser<QSharedPointer<ast::Cond::CondClause>> {
        QSharedPointer<ast::Cond::CondClause> parse(qparsec::Input &input);
    };
    Parser<QSharedPointer<ast::Cond::CondClause>> *CondClause();

    Parser<QList<ast::SharedVal>> *Else();

public:
    ast::SharedVal parse(qparsec::Input &input);
};
qparsec::Parser<ast::SharedVal> *Cond();

}
}
}

#endif // QSCHEME_PARSER_EXPRESSION_PARSERCOND_H
