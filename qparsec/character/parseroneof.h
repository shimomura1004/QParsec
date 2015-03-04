#ifndef QPARSEC_CHARACTER_PARSERONEOF_H
#define QPARSEC_CHARACTER_PARSERONEOF_H

#include <QChar>
#include "parser.h"
#include "parserexception.h"

namespace qparsec {
namespace character {

class ParserOneOf : public Parser<QChar> {
protected:
    QString chars_;

public:
    ParserOneOf(QString chars);
    QChar parse(Input &input);
};

ParserOneOf *OneOf(QString chars);

}
}

#endif // QPARSEC_CHARACTER_PARSERONEOF_H
