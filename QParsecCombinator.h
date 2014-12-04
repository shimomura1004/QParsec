#ifndef QPARSECCOMBINATOR_H
#define QPARSECCOMBINATOR_H

#include "QParsec.h"
#include <QList>

// todo: use smart pointer
// todo: remove parse method (use operator()?)

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

template<typename T>
struct ParserMany : Parser< QList<T> > {
    Parser<T> *p_;

    ParserMany(Parser<T> *p) : p_(p) {}
    ~ParserMany() {delete p_;}

    QList<T> parse(Input &input) {
        QList<T> result;
        try {
            Q_FOREVER {
                result.push_back(p_->parse(input));
            }
        }
        catch (const ParserException &) {
            return result;
        }
    }
};

template<typename T>
struct ParserMany1 : ParserMany<T> {
    ParserMany1(Parser<T> *p) : ParserMany<T>(p) {}

    QList<T> parse(Input &input) {
        QList<T> result;
        result.push_back(ParserMany<T>::p_->parse(input));
        result.append(ParserMany<T>::parse(input));
        return result;
    }
};

template<typename T>
struct ParserChoice : Parser<T> {
  QList< Parser<T>* > ps_;

  ParserChoice(QList< Parser<T>* > ps) : ps_(ps) {}
  ParserChoice(std::initializer_list< Parser<T>* > &ps) : ps_(ps) {}
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

template<typename T, typename TSep>
struct ParserSepBy : Parser< QList<T> > {
    Parser<T> *p_;
    Parser<TSep> *sep_;

    ParserSepBy(Parser<T> *p, Parser<TSep> *sep) : p_(p), sep_(sep) {}
    ~ParserSepBy() {
        delete p_;
        delete sep_;
    }

    QList<T> parse(Input &input) {
        QList<T> result;
        bool endWithSeparator = false;
        try {
            Q_FOREVER {
                result.push_back(p_->parse(input));
                endWithSeparator = false;
                sep_->parse(input);
                endWithSeparator = true;
            }
        }
        catch (const ParserException &exp) {
            if (endWithSeparator)
                throw exp;
            return result;
        }
    }
};


template<typename T>
ParserTry<T> *Try(Parser<T> *p)
{ return new ParserTry<T>(p); }

template<typename T>
ParserMany<T> *Many(Parser<T> *p)
{ return new ParserMany<T>(p); }

template<typename T>
ParserMany1<T> *Many1(Parser<T> *p)
{ return new ParserMany1<T>(p); }

template<typename T>
ParserChoice<T> *Choice(QList< Parser<T>* > p)
{ return new ParserChoice<T>(p); }
template<typename T>
ParserChoice<T> *Choice(std::initializer_list< Parser<T>* > p)
{ return new ParserChoice<T>(p); }

template<typename T, typename TSep>
ParserSepBy<T, TSep> *SepBy(Parser<T> *p, Parser<TSep> *psep)
{ return new ParserSepBy<T, TSep>(p, psep); }

#endif // QPARSECCOMBINATOR_H
