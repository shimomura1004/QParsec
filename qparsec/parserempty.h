#ifndef PARSEREMPTY
#define PARSEREMPTY

#include "parser.h"

namespace qparsec {

class ParserEmpty : public Parser<void> {
    void parse(Input &) {}
};

ParserEmpty *Empty()
{ return new ParserEmpty(); }

}

#endif // PARSEREMPTY

