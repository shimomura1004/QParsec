#include "parserempty.h"

void qparsec::prim::ParserEmpty::parse(qparsec::Input &) {}

qparsec::prim::ParserEmpty *qparsec::prim::Empty()
{ return new ParserEmpty(); }
