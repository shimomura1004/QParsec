#include "parserinteger.h"
#include "character/character.h"
#include "combinator/combinator.h"
#include "parsernatural.h"

namespace qparsec {
namespace number {
using namespace qparsec;
using namespace character;
using namespace combinator;

ParserInteger::ParserInteger() : Parser<int>() {}

int ParserInteger::parse(Input &input) {
    auto sign = Choice({ Char('-'),
                         Option(Char('+'), QChar('+'))
                       })->parse(input);
    auto num = Natural()->parse(input);

    return (sign == '+' ? 1 : -1) * num;
}

ParserInteger *Integer() { return new ParserInteger(); }

}
}
