#ifndef QPARSEC_H
#define QPARSEC_H

#include <cassert>
#include <QString>
#include <QStringList>
#include <QSharedPointer>

// todo: inherit QIODevice
class Input {
protected:
    int index_;
    QString str_;
    QStringList stacks_;

public:
    Input(const QString &v) : index_(0), str_(v) {}

    bool isEmpty(){ return str_.isEmpty(); }
    int length(){ return str_.length(); }
    int index(){ return index_; }
    QCharRef operator[](int idx) { return str_[idx]; }
    const QString &str() { return str_; }

    void preserve() {
        stacks_.push_back("");
    }
    QString consume(int n) {
        const QString s = str_.left(n);
        str_.remove(0, n);
        index_ += s.length();
        if (!stacks_.empty())
            stacks_.last().append(s);
        return s;
    }
    void restore() {
        assert(!stacks_.empty());
        const QString s = stacks_.last();
        str_.prepend(s);
        index_ -= s.length();
        clear();
    }
    void clear() {
        assert(!stacks_.empty());
        stacks_.pop_back();
    }
};

struct ParserException {
    int index;
    QString reason;
    ParserException(int i, QString s) : index(i), reason(s) {}
};

template<typename T>
struct Parser {
    T *out_;

    Parser(T *out = nullptr) : out_(out) {}
    virtual ~Parser() {}
    virtual T parse(Input &input) = 0;

    inline T setOut(T &result) {
        if (out_)
            *out_ = result;
        return result;
    }
};

template<>
struct Parser<void> {
    virtual ~Parser() {}
    virtual void parse(Input &input) = 0;
};

template<typename T>
QSharedPointer< Parser<T> > S(Parser<T> *p)
{ return QSharedPointer< Parser<T> >(p);}

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

    ParserLeft(Parser<T1> *p1, Parser<T2> *p2, T1 *out) : Parser<T1>(out), p1_(p1), p2_(p2) {}
    ~ParserLeft() {
        delete p1_;
        delete p2_;
    }

    T1 parse(Input &input) {
        T1 result = p1_->parse(input);
        p2_->parse(input);
        return Parser<T1>::setOut(result);
    }
};
template<typename T>
struct ParserLeft<T, void> : Parser<T> {
    Parser<T> *p1_;
    Parser<void> *p2_;

    ParserLeft(Parser<T> *p1, Parser<void> *p2, T *out) : Parser<T>(out), p1_(p1), p2_(p2) {}
    ~ParserLeft() {
        delete p1_;
        delete p2_;
    }

    T parse(Input &input) {
        T result = p1_->parse(input);
        p2_->parse(input);
        return Parser<T>::setOut(result);
    }
};

template<typename T1, typename T2>
struct ParserRight : Parser<T2> {
    Parser<T1> *p1_;
    Parser<T2> *p2_;

    ParserRight(Parser<T1> *p1, Parser<T2> *p2, T2 *out) : Parser<T2>(out), p1_(p1), p2_(p2) {}
    ~ParserRight() {
        delete p1_;
        delete p2_;
    }

    T2 parse(Input &input) {
        p1_->parse(input);
        T2 result = p2_->parse(input);
        return Parser<T2>::setOut(result);
    }
};
template<typename T>
struct ParserRight<void, T> : Parser<T> {
    Parser<void> *p1_;
    Parser<T> *p2_;

    ParserRight(Parser<void> *p1, Parser<T> *p2, T *out) : Parser<T>(out), p1_(p1), p2_(p2) {}
    ~ParserRight() {
        delete p1_;
        delete p2_;
    }

    T parse(Input &input) {
        p1_->parse(input);
        T result = p2_->parse(input);
        return Parser<T>::setOut(result);
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

    ParserApply(Parser<T1> *p, T2 (*func)(T1), T2 *out) : Parser<T2>(out), p_(p), func_(func) {}
    virtual ~ParserApply() { delete p_; }

    T2 parse(Input &input) {
        T2 result = func_(p_->parse(input));
        return Parser<T2>::setOut(result);
    }
};

template<typename T>
struct ParserLazy : Parser<T> {
    Parser<T>*(*p_)();

    ParserLazy(Parser<T>* (*p)(), T *out) : Parser<T>(out), p_(p) {}

    T parse(Input &input) {
        T result = p_()->parse(input);
        return Parser<T>::setOut(result);
    }
};

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
ParserApply<T1, T2> *Apply(Parser<T1> *p, T2(*func)(T1), T2 *out = nullptr)
{ return new ParserApply<T1, T2>(p, func, out); }

template<typename T1, typename T2>
ParserLeft<T1, T2> *Left(Parser<T1> *p1, Parser<T2> *p2, T1 *out = nullptr)
{ return new ParserLeft<T1, T2>(p1, p2, out); }

template<typename T1, typename T2>
ParserRight<T1, T2> *Right(Parser<T1> *p1, Parser<T2> *p2, T2 *out = nullptr)
{ return new ParserRight<T1, T2>(p1, p2, out); }

template<typename T>
ParserLazy<T> *Lazy(Parser<T>* (*p)(), T *out = nullptr)
{ return new ParserLazy<T>(p, out); }

#endif // QPARSEC_H
