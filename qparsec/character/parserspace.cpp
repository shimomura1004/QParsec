#include "parserspace.h"

qparsec::Parser<QChar> *qparsec::character::Space()
{ return OneOf(" \v\f\t\r\n"); }

qparsec::Parser<QString> *qparsec::character::Spaces()
{ return combinator::Many(Space()); }

qparsec::Parser<QChar> *qparsec::character::Newline()
{ return Char('\n'); }

qparsec::Parser<QChar> *qparsec::character::Tab()
{ return Char('\t'); }
