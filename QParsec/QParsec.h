#ifndef QPARSEC_H
#define QPARSEC_H

#include <cassert>
#include <QString>
#include <QPair>
#include <QStringList>
#include <QSharedPointer>

namespace qparsec {

// todo: inherit QIODevice
class Input {
protected:
    int index_;
    QString str_;
    QList<uint64_t> indexes_;

public:
    Input(const QString &v) : index_(0), str_(v) {}

    bool isEmpty() { return index_ >= str_.length(); }
    int length() { return str_.length() - index_; }
    int index() { return index_; }

    QCharRef operator[](int idx) { return str_[index_ + idx]; }
    const QString str() { return mid(0); }
    const QString mid(int position, int n = -1) { return str_.mid(index_ + position, n); }
    const QStringRef midRef(int position, int n = -1) { return str_.midRef(index_ + position, n); }
    const QString left(int n) { return mid(n); }

    void preserve() { indexes_.push_back(index_); }
    QString consume(int n) {
        const QString s = str_.mid(index_, n);
        index_ += s.length();
        return s;
    }
    void restore() {
        index_ = indexes_.takeLast();
    }
    void clear() {
        indexes_.pop_back();
    }
};

struct ParserException {
    int index;
    QString reason;
    ParserException(int i, QString s) : index(i), reason(s) {}
};

template<typename T>
struct Parser {
    Parser() {}
    virtual ~Parser() {}
    virtual T parse(Input &input) = 0;
};

template<typename T>
QSharedPointer< Parser<T> > S(Parser<T> *p)
{ return QSharedPointer< Parser<T> >(p);}

struct ParserEmpty : Parser<void> {
    void parse(Input &) {}
};

template<typename T>
struct ParserReturn : Parser<T> {
    T value;
    ParserReturn(T v) : value(v) {}
    T parse(Input &) {
        return value;
    }
};

template<typename T>
struct ParserIgnore : Parser<void> {
    Parser<T> *p_;

    ParserIgnore(Parser<T> *p) : Parser<void>(), p_(p) {}
    ~ParserIgnore() { delete p_; }

    void parse(Input &input) {
        p_->parse(input);
    }
};

template<typename... Ts>
struct ParserSeq : Parser<void> {
    void parse(Input&) {}
};
template<typename T, typename... Ts>
struct ParserSeq<T, Ts...> : ParserSeq<Ts...> {
    Parser<T>* p_;

    ParserSeq(Parser<T>* p, Parser<Ts>*... ps) : ParserSeq<Ts...>(ps...), p_(p) {}
    virtual ~ParserSeq() { delete p_; }

    void parse(Input &input) {
        p_->parse(input);
        ParserSeq<Ts...>::parse(input);
    }
};

struct ParserFail : Parser<void> {
    QString message_;

    ParserFail(QString message) : message_(message) {}

    void parse(Input &input) {
        throw ParserException(input.index(), message_);
    }
};

template<typename T>
struct ParserTry : Parser<T> {
    Parser<T> *p_;

    ParserTry(Parser<T> *p) : p_(p) {}
    virtual ~ParserTry() {delete p_;}

    T parse(Input &input) {
        input.preserve();
        try {
            auto result = p_->parse(input);
            input.clear();
            return result;
        }
        catch (const ParserException &e) {
            input.restore();
            throw e;
        }
    }
};

template<>
struct ParserTry<void> : Parser<void> {
    Parser<void> *p_;

    ParserTry(Parser<void> *p) : p_(p) {}
    virtual ~ParserTry() {delete p_;}

    void parse(Input &input) {
        input.preserve();
        try {
            p_->parse(input);
            input.clear();
            return;
        }
        catch (const ParserException &e) {
            input.restore();
            throw e;
        }
    }
};

template<typename T1, typename T2>
struct ParserLeft : Parser<T1> {
    Parser<T1> *p1_;
    Parser<T2> *p2_;

    ParserLeft(Parser<T1> *p1, Parser<T2> *p2) : Parser<T1>(), p1_(p1), p2_(p2) {}
    ~ParserLeft() {
        delete p1_;
        delete p2_;
    }

    T1 parse(Input &input) {
        T1 result = p1_->parse(input);
        p2_->parse(input);
        return result;
    }
};

template<typename T1, typename T2>
struct ParserRight : Parser<T2> {
    Parser<T1> *p1_;
    Parser<T2> *p2_;

    ParserRight(Parser<T1> *p1, Parser<T2> *p2) : Parser<T2>(), p1_(p1), p2_(p2) {}
    ~ParserRight() {
        delete p1_;
        delete p2_;
    }

    T2 parse(Input &input) {
        p1_->parse(input);
        T2 result = p2_->parse(input);
        return result;
    }
};

template<typename T1, typename T2>
struct ParserPair : Parser<QPair<T1, T2>> {
    Parser<T1> *p1_;
    Parser<T2> *p2_;

    ParserPair(Parser<T1> *p1, Parser<T2> *p2) : Parser<QPair<T1, T2>>(), p1_(p1), p2_(p2) {}
    ~ParserPair() {
        delete p1_;
        delete p2_;
    }

    QPair<T1, T2> parse(Input &input) {
        T1 left = p1_->parse(input);
        T2 right = p2_->parse(input);
        return QPair<T1, T2>(left, right);
    }
};

template<typename T>
struct ParserHelp : Parser<T> {
    Parser<T> *p_;
    QString message_;

    ParserHelp(Parser<T> *p, const QString &message) : p_(p), message_(message) {}
    virtual ~ParserHelp() {delete p_;}

    T parse(Input &input) {
        try {
            return p_->parse(input);
        }
        catch (const ParserException &e) {
            ParserException e2(e.index, message_);
            throw e2;
        }
    }
};

template<typename T1, typename T2>
struct ParserApply : Parser<T2> {
    Parser<T1> *p_;
    T2 (*func_)(T1);

    ParserApply(Parser<T1> *p, T2 (*func)(T1)) : Parser<T2>(), p_(p), func_(func) {}
    virtual ~ParserApply() { delete p_; }

    T2 parse(Input &input) {
        return func_(p_->parse(input));
    }
};
template<typename T2>
struct ParserApply<void, T2> : Parser<T2> {
    Parser<void> *p_;
    T2 (*func_)();

    ParserApply(Parser<void> *p, T2 (*func)()) : Parser<T2>(), p_(p), func_(func) {}
    virtual ~ParserApply() { delete p_; }

    T2 parse(Input &input) {
        return p_->parse(input);
    }
};

template<typename T1, typename T2, typename T3>
struct ParserApply2 : Parser<T2> {
    Parser<T1> *p1_;
    Parser<T2> *p2_;
    T3(*func_)(T1, T2);

    ParserApply2(Parser<T1> *p1, Parser<T2> *p2, T3 (*func)(T1, T2)) : Parser<T3>(), p1_(p1), p2_(p2), func_(func) {}
    virtual ~ParserApply2() {
        delete p1_;
        delete p2_;
    }

    T3 parse(Input &input) {
        auto r1 = p1_->parse(input);
        auto r2 = p2_->parse(input);
        return func_(r1, r2);
    }
};

template<typename T>
struct ParserLazy : Parser<T> {
    Parser<T>*(*p_)();

    ParserLazy(Parser<T>* (*p)()) : Parser<T>(), p_(p) {}

    T parse(Input &input) {
        return p_()->parse(input);
    }
};

ParserEmpty *Empty()
{ return new ParserEmpty(); }

template<typename T>
ParserReturn<T> *Return(T v)
{ return new ParserReturn<T>(v); }

template<typename T>
ParserIgnore<T> *Ignore(Parser<T> *p)
{ return new ParserIgnore<T>(p); }

template<typename... Ts>
ParserSeq<Ts...> *Seq(Parser<Ts>*... ps)
{ return new ParserSeq<Ts...>(ps...); }

ParserFail *Fail(QString message)
{ return new ParserFail(message); }

template<typename T>
ParserTry<T> *Try(Parser<T> *p)
{ return new ParserTry<T>(p); }

template<typename T>
ParserHelp<T> *Help(Parser<T> *p, const QString &message)
{ return new ParserHelp<T>(p, message); }

template<typename T1, typename T2>
ParserApply<T1, T2> *Apply(Parser<T1> *p, T2(*func)(T1))
{ return new ParserApply<T1, T2>(p, func); }

template<typename T1, typename T2, typename T3>
ParserApply2<T1, T2, T3> *Apply2(Parser<T1> *p1, Parser<T2> *p2, T3(*func)(T1, T2))
{ return new ParserApply2<T1, T2, T3>(p1, p2, func); }

template<typename T1, typename T2>
ParserLeft<T1, T2> *Left(Parser<T1> *p1, Parser<T2> *p2)
{ return new ParserLeft<T1, T2>(p1, p2); }

template<typename T1, typename T2>
ParserRight<T1, T2> *Right(Parser<T1> *p1, Parser<T2> *p2)
{ return new ParserRight<T1, T2>(p1, p2); }

template<typename T1, typename T2>
ParserPair<T1, T2> *Pair(Parser<T1> *p1, Parser<T2> *p2)
{ return new ParserPair<T1, T2>(p1, p2); }

template<typename T>
ParserLazy<T> *Lazy(Parser<T>* (*p)())
{ return new ParserLazy<T>(p); }

}

#endif // QPARSEC_H
