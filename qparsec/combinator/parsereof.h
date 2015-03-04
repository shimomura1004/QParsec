#ifndef PARSEREOF
#define PARSEREOF

#include "parser.h"
#include "parserexception.h"

namespace qparsec {
namespace combinator {

class ParserEof : public Parser<void> {
public:
    void parse(Input &input) {
        if (!input.isEmpty())
            throw ParserException(input.index(), "Isn't at the end of input");
    }
};

ParserEof *Eof()
{ return new ParserEof(); }

}
}
#endif // PARSEREOF

