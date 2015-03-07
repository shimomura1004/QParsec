#ifndef QSCHEME_PARSER_EXPRESSION_PARSERCASE_H
#define QSCHEME_PARSER_EXPRESSION_PARSERCASE_H

#include "parser.h"
#include "ast/ast.h"

namespace qscheme {
namespace parser {
namespace expression {

class ParserCase : public qparsec::Parser<ast::SharedVal> {
protected:
    struct ParserCaseClause : Parser<QSharedPointer<ast::Case::CaseClause>> {
        QSharedPointer<ast::Case::CaseClause> parse(qparsec::Input &input);
    };
    Parser<QSharedPointer<ast::Case::CaseClause>> *CaseClause();
    Parser<QList<ast::SharedVal>> *Else();

public:
    ast::SharedVal parse(qparsec::Input &input);
};

qparsec::Parser<ast::SharedVal> *Case();

}
}
}

#endif // QSCHEME_PARSER_EXPRESSION_PARSERCASE_H
