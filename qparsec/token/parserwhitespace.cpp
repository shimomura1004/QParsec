#include "parserwhitespace.h"

#include "character/character.h"
#include "combinator/combinator.h"

namespace qparsec {
namespace token {

using namespace character;
using namespace combinator;

Parser<void> *WhiteSpace() { return SkipMany(Space()); }

}
}
