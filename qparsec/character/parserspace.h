#ifndef PARSERSPACE
#define PARSERSPACE

#include "parserchar.h"
#include "parseroneof.h"

namespace qparsec {
namespace character {

Parser<QChar> *Space();

//Parser<QString> *Spaces()
//{ return combinators::Many(Space()); }

Parser<QChar> *Newline();
Parser<QChar> *Tab();

}
}

#endif // PARSERSPACE

