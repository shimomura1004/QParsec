#ifndef QPARSEC_NUMBER_PARSERDOUBLE_H
#define QPARSEC_NUMBER_PARSERDOUBLE_H

#include "parser.h"

namespace qparsec {
namespace number {

class ParserDouble : public Parser<double> {
public:
    ParserDouble();
    double parse(Input &input);
};

ParserDouble *Double();

}
}

#endif // QPARSEC_NUMBER_PARSERDOUBLE_H
