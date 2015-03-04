#ifndef PARSER_H
#define PARSER_H

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

#endif // PARSER_H
