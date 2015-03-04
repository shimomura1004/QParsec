#ifndef PARSERNONEOF_H
#define PARSERNONEOF_H

#include <QChar>
#include <QString>
#include "parser.h"
#include "parserexception.h"

namespace qparsec {
namespace character {

class ParserNoneOf : public Parser<QChar> {
protected:
    QString chars_;

public:
    ParserNoneOf(QString chars);
    QChar parse(Input &input);
};

ParserNoneOf *NoneOf(QString chars);

}
}

#endif // PARSERNONEOF_H
