#ifndef PARSERCHOICE
#define PARSERCHOICE

#include "parser.h"

namespace qparsec {
namespace combinator {

template<typename T>
class ParserChoice : public Parser<T> {
protected:
    QList< Parser<T>* > ps_;

public:
    ParserChoice(QList< Parser<T>* > ps) : Parser<T>(), ps_(ps) {}
    ParserChoice(std::initializer_list< Parser<T>* > &ps) : Parser<T>(), ps_(ps) {}
    ~ParserChoice() {
        Q_FOREACH(auto &p, ps_) {
            delete p;
        }
        ps_.clear();
    }

    T parse(Input &input) {
        ParserException exp(0, "");
        Q_FOREACH(Parser<T> *p, ps_) {
            try {
                return p->parse(input);
            }
            catch (const ParserException &e) {
                exp = e;
                continue;
            }
        }
        throw exp;
    }
};

template<typename T>
ParserChoice<T> *Choice(QList< Parser<T>* > p)
{ return new ParserChoice<T>(p); }

template<typename T>
ParserChoice<T> *Choice(std::initializer_list< Parser<T>* > ps)
{ return new ParserChoice<T>(ps); }

}
}

#endif // PARSERCHOICE

