#ifndef PARSERLEXEME_H
#define PARSERLEXEME_H

#include "parser.h"
#include "parserwhitespace.h"

namespace qparsec {
namespace token {

template<typename T>
class ParserLexeme : public Parser<T> {
protected:
    Parser<T> *p_;

public:
    ParserLexeme(Parser<T> *p) : Parser<T>(), p_(p) {}
    ~ParserLexeme() { delete p_; }

    T parse(Input &input) {
        auto result = p_->parse(input);
        WhiteSpace()->parse(input);
        return result;
    }
};

template<typename T>
Parser<T> *Lexeme(Parser<T> *p)
{ return new ParserLexeme<T>(p); }

}
}

#endif // PARSERLEXEME_H
