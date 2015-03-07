#include "parserdouble.h"
#include "character/character.h"
#include "combinator/combinator.h"
#include "parsernatural.h"
#include "parserdecimal.h"

namespace qparsec {
namespace number {
using namespace qparsec;
using namespace character;
using namespace combinator;

ParserDouble::ParserDouble() : Parser<double>() {}

double ParserDouble::parse(Input &input) {
    auto sign_b = Choice({ Char('-'),
                           Option(Char('+'), QChar('+'))
                         })->parse(input);
    auto num_b = Option(Natural(), 0)->parse(input);

    try
    {
        Char('.')->parse(input);
    }
    catch (const ParserException &){
        OneOf("eE")->parse(input);
        auto sign_e = Choice({ Char('-'),
                               Option(Char('+'), QChar('+'))
                             })->parse(input);
        auto num_e = Natural()->parse(input);
        return QStringLiteral("%1%2e%3%4").arg(sign_b,
                                               QString::number(num_b),
                                               sign_e,
                                               QString::number(num_e)).toDouble();
    }
    auto num_e = Decimal()->parse(input);

    return QStringLiteral("%1%2.%3").arg(sign_b,
                                         QString::number(num_b),
                                         QString::number(num_e)).toDouble();
}

ParserDouble *Double() { return new ParserDouble(); }

}
}
