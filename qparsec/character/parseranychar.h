#ifndef QPARSEC_CHARACTER_PARSERANYCHAR_H
#define QPARSEC_CHARACTER_PARSERANYCHAR_H

#include "parser.h"
#include "parserexception.h"

namespace qparsec {
namespace character {

class ParserAnyChar : public Parser<QChar> {
public:
    ParserAnyChar();
    QChar parse(Input &input);
};

ParserAnyChar *AnyChar();

}
}

#endif // QPARSEC_CHARACTER_PARSERANYCHAR_H
