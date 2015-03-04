#ifndef PARSERSATISFY_H
#define PARSERSATISFY_H

#include "QChar"
#include "parser.h"
#include "parserexception.h"

namespace qparsec {
namespace character {

class ParserSatisfy : public Parser<QChar> {
protected:
    bool(*judge_)(QChar);

public:
    ParserSatisfy(bool(*judge)(QChar));
    QChar parse(Input &input);
};

Parser<QChar> *Satisfy(bool(*judge)(QChar));

}
}

#endif // PARSERSATISFY_H
