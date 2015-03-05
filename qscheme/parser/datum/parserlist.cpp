#include "parserlist.h"
#include "character/character.h"
#include "combinator/combinator.h"
#include "token/parserlexeme.h"
#include "datum.h"

namespace qscheme {
namespace parser {
namespace datum {

using namespace qparsec;
using namespace character;
using namespace combinator;
using namespace token;

ast::SharedVal ParserList::ParserAbbriviation::parse(qparsec::Input &input) {
    try {
        Char('\'')->parse(input);
        auto q = ast::Variable::create("quote");
        auto datum = Datum()->parse(input);
        return ast::List::create(QList<ast::SharedVal>({q, datum}));
    } catch (const ParserException &) {}

    try {
        Char('`')->parse(input);
        auto q = ast::Variable::create("quasiquote");
        auto datum = Datum()->parse(input);
        return ast::List::create(QList<ast::SharedVal>({q, datum}));
    } catch (const ParserException &) {}

    try {
        Str(",@")->parse(input);
        auto q = ast::Variable::create("unquote-splicing");
        auto datum = Datum()->parse(input);
        return ast::List::create(QList<ast::SharedVal>({q, datum}));
    } catch (const ParserException &) {}

    Char(',')->parse(input);
    auto q = ast::Variable::create("unquote");
    auto datum = Datum()->parse(input);
    return ast::List::create(QList<ast::SharedVal>({q, datum}));
}

Parser<ast::SharedVal> *ParserList::Abbriviation() { return new ParserAbbriviation(); }

ast::SharedVal ParserList::parse(qparsec::Input &input) {
    try {
        return Abbriviation()->parse(input);
    } catch (const ParserException &) {}

    Char('(')->parse(input);

    try {
        auto data = Many1(Datum())->parse(input);

        try {
            Lexeme(Char('.'))->parse(input);
            auto datum = Datum()->parse(input);
            Lexeme(Char(')'))->parse(input);
            return ast::DList::create(QList<ast::SharedVal>(data), datum);
        } catch (const ParserException &) {
            Lexeme(Char(')'))->parse(input);
            return ast::List::create(QList<ast::SharedVal>(data));
        }

    } catch (const ParserException &) {
        Lexeme(Char(')'))->parse(input);
        return ast::List::create(QList<ast::SharedVal>());
    }
}

Parser<ast::SharedVal> *List() { return new ParserList(); }

}
}
}
