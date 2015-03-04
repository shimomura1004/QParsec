#ifndef QPARSEC_COMBINATOR_PARSEREOF
#define QPARSEC_COMBINATOR_PARSEREOF

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
#endif // QPARSEC_COMBINATOR_PARSEREOF

