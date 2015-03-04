#ifndef QPARSEC_PARSER_H
#define QPARSEC_PARSER_H

#include "input.h"

namespace qparsec {

template <typename T>
class Parser
{
public:
    Parser() {}
    virtual ~Parser() {}
    virtual T parse(Input &input) = 0;
};

}

#endif // QPARSEC_PARSER_H
