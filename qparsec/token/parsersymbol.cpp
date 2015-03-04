#include "parsersymbol.h"
#include "character/character.h"
#include "parserlexeme.h"

namespace qparsec {
namespace token {

using namespace character;

Parser<QString> *Symbol(QString str)
{ return Lexeme(Str(str)); }

}
}
