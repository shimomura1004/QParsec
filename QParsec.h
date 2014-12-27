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

    inline T setOut(T result) {
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

#endif // QPARSEC_H
