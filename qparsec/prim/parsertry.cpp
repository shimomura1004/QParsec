#include "parsertry.h"

namespace qparsec {
namespace prim {

ParserTry<void>::ParserTry(Parser<void> *p) : p_(p) {}

ParserTry<void>::~ParserTry() {delete p_;}

void ParserTry<void>::parse(Input &input) {
    input.preserve();
    try {
        p_->parse(input);
        input.clear();
        return;
    }
    catch (const ParserException &e) {
        input.restore();
        throw e;
    }
}

}
}
