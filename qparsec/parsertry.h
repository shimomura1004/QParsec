#ifndef PARSERTRY
#define PARSERTRY

#include "parser.h"
#include "parserexception.h"

namespace qparsec {

template<typename T>
class ParserTry : public Parser<T> {
protected:
    Parser<T> *p_;

public:
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

template<>
class ParserTry<void> : public Parser<void> {
protected:
    Parser<void> *p_;

public:
    ParserTry(Parser<void> *p) : p_(p) {}
    ~ParserTry() {delete p_;}

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
ParserTry<T> *Try(Parser<T> *p)
{ return new ParserTry<T>(p); }

}

#endif // PARSERTRY

