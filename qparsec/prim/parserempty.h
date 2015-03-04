#ifndef QPARSEC_PRIM_PARSEREMPTY
#define QPARSEC_PRIM_PARSEREMPTY

#include "parser.h"

namespace qparsec {
namespace prim {

class ParserEmpty : public Parser<void> {
    void parse(Input &);
};

ParserEmpty *Empty();

}
}

#endif // QPARSEC_PRIM_PARSEREMPTY

