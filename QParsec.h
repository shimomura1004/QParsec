#ifndef QPARSEC_H
#define QPARSEC_H

#include <QString>

struct Input {
    int index;
    QString value;
    Input(QString v) : index(0), value(v) {}
};

struct ParserException {
    int index;
    QString reason;
    ParserException(int i, QString s) : index(i), reason(s) {}
};

template<typename R>
struct Parser {
    virtual R parse(Input &input) = 0;
    virtual ~Parser() {}
};

#endif // QPARSEC_H
