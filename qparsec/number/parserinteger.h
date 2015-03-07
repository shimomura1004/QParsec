#ifndef QPARSEC_NUMBER_PARSERINTEGER_H
#define QPARSEC_NUMBER_PARSERINTEGER_H

#include "parser.h"

namespace qparsec {
namespace number {

class ParserInteger : public Parser<int> {
public:
    ParserInteger();
    int parse(Input &input);
};

ParserInteger *Integer();

}
}

#endif // QPARSEC_NUMBER_PARSERINTEGER_H
