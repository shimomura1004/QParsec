#include "parserlambda.h"
#include "expression.h"
#include "prim/prim.h"
#include "character/character.h"
#include "combinator/combinator.h"
#include "token/token.h"
#include "language/scheme/scheme.h"

#include <QStringList>

namespace qscheme {
namespace parser {
namespace expression {
using namespace qparsec;
using namespace prim;
using namespace character;
using namespace combinator;
using namespace token;
using namespace language;

QPair<QStringList, QString> ParserLambda::ParserFormals::parse(Input &input) {
    try {
        auto vars = Try(Parens(Many(Lexeme(scheme::Variable()))))->parse(input);
        return QPair<QStringList, QString>(vars, "");
    } catch (const ParserException &) {}

    try {
        auto var = Lexeme(scheme::Variable())->parse(input);
        return QPair<QStringList, QString>(QStringList(), var);
    } catch (const ParserException &) {}

    Lexeme(Char('('))->parse(input);
    auto vars = Many1(Lexeme(scheme::Variable()))->parse(input);
    Lexeme(Char('.'))->parse(input);
    auto var = Lexeme(scheme::Variable())->parse(input);
    Lexeme(Char(')'))->parse(input);

    return QPair<QStringList, QString>(vars, var);
}

Parser<QPair<QStringList, QString> > *ParserLambda::Formals() {
    return new ParserFormals();
}

ast::SharedVal ParserLambda::parse(Input &input) {
    Lexeme(Char('('))->parse(input);
    Lexeme(Str("lambda"))->parse(input);
    QPair<QStringList, QString> formals = Lexeme(Formals())->parse(input);
    auto body = Lexeme(Body())->parse(input);
    Lexeme(Char(')'))->parse(input);

    return ast::Lambda::create(formals.first, formals.second, body, ast::Env());
}

Parser<ast::SharedVal> *Lambda() { return new ParserLambda(); }

}
}
}
