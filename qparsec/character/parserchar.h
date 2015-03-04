#ifndef PARSERCHAR_H
#define PARSERCHAR_H

#include <QChar>
#include "parser.h"
#include "parserexception.h"
#include "parseroneof.h"

namespace qparsec {
namespace character {

class ParserChar : public Parser<QChar> {
protected:
    QChar c_;

public:
    ParserChar(QChar c);
    QChar parse(Input &input);
};

ParserChar *Char(QChar c);
Parser<QChar> *Upper();
Parser<QChar> *Lower();
Parser<QChar> *Alphanum();
Parser<QChar> *Letter();
Parser<QChar> *Digit();
Parser<QChar> *Hexadigit();
Parser<QChar> *Octdigit();

}
}

#endif // PARSERCHAR_H
