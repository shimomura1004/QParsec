#include "parsercomma.h"
#include "character/character.h"

namespace qparsec {
namespace token {

using namespace character;

Parser<QChar> *Comma() { return Lexeme(Char(',')); }

}
}
