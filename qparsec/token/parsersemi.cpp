#include "parsersemi.h"
#include "character/character.h"

namespace qparsec {
namespace token {

using namespace character;

Parser<QChar> *Semi() { return Lexeme(Char(';')); }

}
}
