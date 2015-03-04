#ifndef PARSERFAIL
#define PARSERFAIL

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

#endif // PARSERFAIL

