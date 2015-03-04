#ifndef PARSEREXCEPTION_H
#define PARSEREXCEPTION_H

#include <QString>

namespace qparsec {

class ParserException
{
protected:
    int index_;
    QString reason_;

public:
    ParserException(int index, QString reason);
    int index() const {return index_;}
    QString reason() const {return reason_;}
};

}

#endif // PARSEREXCEPTION_H
