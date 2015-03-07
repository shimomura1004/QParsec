#ifndef QPARSEC_CHARACTER_PARSERSPACE
#define QPARSEC_CHARACTER_PARSERSPACE

#include "parserchar.h"
#include "parseroneof.h"
#include "combinator/parsermany.h"

namespace qparsec {
namespace character {

Parser<QChar> *Space();
Parser<QString> *Spaces();
Parser<QChar> *Newline();
Parser<QChar> *Tab();

}
}

#endif // QPARSEC_CHARACTER_PARSERSPACE

