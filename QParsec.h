#ifndef QPARSEC_H
#define QPARSEC_H

#include <QString>
#include <QSharedPointer>

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
    virtual R operator()(Input &input) {return parse(input);}
};

template<typename T>
QSharedPointer< Parser<T> > S(Parser<T> *p)
{ return QSharedPointer< Parser<T> >(p);}

#endif // QPARSEC_H
