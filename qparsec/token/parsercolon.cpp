#include "parsercolon.h"
#include "character/character.h"
#include "parserlexeme.h"

namespace qparsec {
namespace token {

using namespace character;

Parser<QChar> *Colon() { return Lexeme(Char(':')); }

}
}
