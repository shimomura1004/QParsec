#ifndef QPARSEC_PRIM_PARSERFAIL
#define QPARSEC_PRIM_PARSERFAIL

#include "parser.h"

namespace qparsec {
namespace prim {

class ParserFail : public Parser<void> {
protected:
    QString message_;

public:
    ParserFail(QString message);
    void parse(Input &input);
};

ParserFail *Fail(QString message);

}
}

#endif // QPARSEC_PRIM_PARSERFAIL

