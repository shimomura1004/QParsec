#ifndef PARSEREOF
#define PARSEREOF

#include "parser.h"
#include "parserexception.h"

namespace qparsec {
namespace combinator {

class ParserEof : public Parser<void> {
public:
    void parse(Input &input);
};

ParserEof *Eof();

}
}
#endif // PARSEREOF

