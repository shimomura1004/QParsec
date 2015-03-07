#include "parserdo.h"
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

QSharedPointer<ast::Do::IterationSpec> ParserDo::ParserIterationSpec::parse(qparsec::Input &input) {
    Lexeme(Char('('))->parse(input);
    auto var = Lexeme(scheme::Variable())->parse(input);
    auto init = Expression()->parse(input);

    try {
        auto step = Expression()->parse(input);
        Lexeme(Char(')'))->parse(input);
        return ast::Do::IterationSpecWithStep::create(var, init, step);
    }
    catch (const ParserException&) {
        Lexeme(Char(')'))->parse(input);
        return ast::Do::IterationSpec::create(var, init);
    }
}

Parser<QSharedPointer<ast::Do::IterationSpec> > *ParserDo::IterationSpec() { return new ParserIterationSpec(); }

ast::SharedVal ParserDo::parse(qparsec::Input &input) {
    Lexeme(Char('('))->parse(input);
    Lexeme(Str("do"))->parse(input);
    auto iterationSpecs = Lexeme(Parens(Many(IterationSpec())))->parse(input);
    auto test_doresult = Lexeme(Parens(Pair(Expression(), Expression())))->parse(input);
    auto commands = Lexeme(Many(Expression()))->parse(input);
    Lexeme(Char(')'))->parse(input);

    return ast::Do::create(iterationSpecs, test_doresult.first, test_doresult.second, commands);
}

qparsec::Parser<ast::SharedVal> *Do() {
    return new ParserDo();
}

}
}
}
