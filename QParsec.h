#ifndef QPARSEC_H
#define QPARSEC_H

#include <QString>
#include <QSharedPointer>

// todo: define accessor
// duplicate all input (in Try) is inefficient
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

template<typename T>
struct Parser {
    virtual T parse(Input &input) = 0;
    virtual ~Parser() {}
    virtual T operator()(Input &input) {return parse(input);}
};

template<typename T>
QSharedPointer< Parser<T> > S(Parser<T> *p)
{ return QSharedPointer< Parser<T> >(p);}

// todo: handle arbitrary number of arguments
template<typename T1, typename T2>
struct ParserSeq : Parser<T2> {
    Parser<T1> *p1_;
    Parser<T2> *p2_;

    ParserSeq(Parser<T1> *p1, Parser<T2> *p2) : p1_(p1), p2_(p2) {}
    ~ParserSeq(){ delete p1_; delete p2_; }

    T2 parse(Input &input) {
        p1_->parse(input);
        return p2_->parse(input);
    }
};

struct ParserFail : Parser<void> {
    QString message_;

    ParserFail(QString message) : message_(message) {}

    void parse(Input &input) {
        throw ParserException(input.index, message_);
    }
};

template<typename T>
struct ParserTry : Parser<T> {
    Parser<T> *p_;

    ParserTry(Parser<T> *p) : p_(p) {}
    ~ParserTry() {delete p_;}

    T parse(Input &input) {
        Input tmp = input;
        try {
            return p_->parse(input);
        }
        catch (const ParserException &e) {
            input = tmp;
            throw e;
        }
    }
};


template<typename T1, typename T2>
ParserSeq<T1, T2> *Seq(Parser<T1> *p1, Parser<T2> *p2)
{ return new ParserSeq<T1, T2>(p1, p2); }

ParserFail *Fail(QString message)
{ return new ParserFail(message); }

template<typename T>
ParserTry<T> *Try(Parser<T> *p)
{ return new ParserTry<T>(p); }

#endif // QPARSEC_H
