#ifndef PARSEREMPTY
#define PARSEREMPTY

#include "parser.h"

namespace qparsec {
namespace prim {

class ParserEmpty : public Parser<void> {
    void parse(Input &);
};

ParserEmpty *Empty();

}
}

#endif // PARSEREMPTY

