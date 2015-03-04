#include "parserspace.h"

qparsec::Parser<QChar> *qparsec::character::Space()
{ return OneOf(" \v\f\t\r\n"); }

qparsec::Parser<QChar> *qparsec::character::Newline()
{ return Char('\n'); }

qparsec::Parser<QChar> *qparsec::character::Tab()
{ return Char('\t'); }
