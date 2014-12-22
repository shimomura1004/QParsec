#ifndef QPARSEC_H
#define QPARSEC_H

#include <cassert>
#include <QString>
#include <QStringList>
#include <QSharedPointer>

class Input {
protected:
    int index_;
    QString value_;
    QStringList stacks_;

public:
    Input(const QString &v) : index_(0), value_(v) {}

    bool isEmpty(){ return value_.isEmpty(); }
    int length(){ return value_.length(); }
    int index(){ return index_; }
    QCharRef operator[](int idx) { return value_[idx]; }
    const QString &str() { return value_; }

    void preserve() {
        stacks_.push_back("");
    }
    QString consume(int n) {
        const QString s = value_.left(n);
        value_.remove(0, n);
        index_ += s.length();
        if (!stacks_.empty())
            stacks_.last().append(s);
        return s;
    }
    void restore() {
        assert(!stacks_.empty());
        const QString s = stacks_.last();
        value_.prepend(s);
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
    ~ParserTry() {delete p_;}

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

template<typename... Ts>
ParserSeq<Ts...> *Seq(Parser<Ts>*... ps)
{ return new ParserSeq<Ts...>(ps...); }

ParserFail *Fail(QString message)
{ return new ParserFail(message); }

template<typename T>
ParserTry<T> *Try(Parser<T> *p)
{ return new ParserTry<T>(p); }

#endif // QPARSEC_H
