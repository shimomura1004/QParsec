#include "parseridentifier.h"

namespace qparsec {
namespace language {
namespace scheme {

Parser<QChar> *ParserIdentifier::Initial() { return Choice({Letter(), SpecialInitial()}); }
Parser<QChar> *ParserIdentifier::SpecialInitial() { return OneOf("!#$%&|*+-/:<=>?@^_~"); }
Parser<QChar> *ParserIdentifier::Subsequent() { return Choice({Initial(), Digit(), SpecialSubsequent()}); }
Parser<QChar> *ParserIdentifier::SpecialSubsequent() { return OneOf("+-.@"); }
Parser<QString> *ParserIdentifier::PeculiarIdentifier() { return Choice({Str("+"), Str("-"), Str("...")}); }

ParserIdentifier::ParserIdentifier() : Parser<QString>() {}

QString ParserIdentifier::parse(qparsec::Input &input) {
    try {
        auto c = Initial()->parse(input);
        auto cs = Many(Subsequent())->parse(input);
        return c + cs;
    }
    catch (const ParserException &) {
        return PeculiarIdentifier()->parse(input);
    }
}

Parser<QString> *Identifier() { return new ParserIdentifier(); }

}
}
}
