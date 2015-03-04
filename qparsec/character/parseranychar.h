#ifndef PARSERANYCHAR_H
#define PARSERANYCHAR_H

#include "parser.h"
#include "parserexception.h"

namespace qparsec {
namespace character {

class ParserAnyChar : public Parser<QChar> {
public:
    ParserAnyChar();
    QChar parse(Input &input);
};

ParserAnyChar *AnyChar()
{ return new ParserAnyChar(); }

}
}

#endif // PARSERANYCHAR_H
