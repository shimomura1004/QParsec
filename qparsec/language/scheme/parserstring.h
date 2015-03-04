#ifndef QPARSEC_LANGUAGE_SCHEME_PARSERSTRING_H
#define QPARSEC_LANGUAGE_SCHEME_PARSERSTRING_H

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

#endif // QPARSEC_LANGUAGE_SCHEME_PARSERSTRING_H
