#include "parserlet.h"
#include "expression.h"
#include "prim/prim.h"
#include "character/character.h"
#include "token/token.h"
#include "language/scheme/scheme.h"

namespace qscheme {
namespace parser {
namespace expression {
using namespace qparsec;
using namespace prim;
using namespace character;
using namespace token;
using namespace language;

ast::SharedVal ParserLet::parse(Input &input) {
    Lexeme(Char('('))->parse(input);

    ast::Let::Type type = Lexeme(Choice({ Try(Right(Str("letrec"), Return(ast::Let::Rec))),
                                          Try(Right(Str("let*"), Return(ast::Let::Star))),
                                          Try(Right(Str("let"), Return(ast::Let::Normal))),
                                        }))->parse(input);

    QString name;
    if (type == ast::Let::Normal) {
        name = Option(Lexeme(scheme::Variable()), QString(""))->parse(input);
    }

    QList<QPair<QString, ast::SharedVal>> bindingspec = Lexeme(Parens(Many(BindingSpec())))->parse(input);
    QList<ast::SharedVal> body = Lexeme(Body())->parse(input);
    Lexeme(Char(')'))->parse(input);

    return ast::Let::create(name, bindingspec, body, type);
}

Parser<ast::SharedVal> *Let() { return new ParserLet(); }

}
}
}
