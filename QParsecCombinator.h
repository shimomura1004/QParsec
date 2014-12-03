#ifndef QPARSECCOMBINATOR_H
#define QPARSECCOMBINATOR_H

#include "QParsec.h"
#include <QList>

template<typename R>
struct Many : Parser< QList<R> > {
    Parser<R> *p_;

    Many(Parser<R> *p) : p_(p) {}
    ~Many() {delete p_;}

    QList<R> parse(Input &input) {
        QList<R> result;
        try {
            while(true) {
                result.push_back(p_->parse(input));
            }
        }
        catch (const ParserException &) {
            return result;
        }
    }
};

template<typename R>
struct Many1 : Many<R> {
    Many1(Parser<R> *p) : Many<R>(p) {}

    QList<R> parse(Input &input) {
        QList<R> result;
        result.push_back(Many<R>::p_->parse(input));
        result.append(Many<R>::parse(input));
        return result;
    }
};

template<typename R>
struct Try : Parser<R> {
    Parser<R> *p_;

    Try(Parser<R> *p) : p_(p) {}
    ~Try() {delete p_;}

    R parse(Input &input) {
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

template<typename R>
struct Choice : Parser<R> {
  QList< Parser<R>* > ps_;

  Choice(QList< Parser<R>* > ps) : ps_(ps) {}
  ~Choice() {
      Q_FOREACH(Parser<R> *p, ps_)
          delete p;
      ps_.clear();
  }

  R parse(Input &input) {
      ParserException exp(0, "");
      Q_FOREACH(Parser<R> *p, ps_) {
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

#endif // QPARSECCOMBINATOR_H
