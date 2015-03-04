#ifndef PARSERSTRING_H
#define PARSERSTRING_H

#include "qparsec.h"
#include "prim/prim.h"
#include "character/character.h"
#include "combinator/combinator.h"

namespace qparsec {
namespace language {
namespace scheme {

Parser<QString> *String();

}
}
}

#endif // PARSERSTRING_H
