#ifndef QPARSEC_PRIM_PARSERIGNORE_H
#define QPARSEC_PRIM_PARSERIGNORE_H

#include "parser.h"

namespace qparsec {
namespace prim {

template<typename T>
class ParserIgnore : public Parser<void>
{
protected:
    Parser<T> *parser_;

public:
    ParserIgnore(Parser<T> *parser) : Parser<void>(), parser_(parser) {}
    ~ParserIgnore() { delete parser_; }

    void parse(Input &input) {
        parser_->parse(input);
    }
};

template<typename T>
ParserIgnore<T> *Ignore(Parser<T> *p)
{ return new ParserIgnore<T>(p); }

}
}

#endif // QPARSEC_PRIM_PARSERIGNORE_H
