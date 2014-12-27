#ifndef QPARSECCOMBINATOR_H
#define QPARSECCOMBINATOR_H

#include "QParsec.h"
#include <QList>

// todo: use Qt default deleting strategy by using setParent
//       or use sharedpointer for all parsers

template<typename T>
struct ParserMany : Parser< QList<T> > {
    Parser<T> *p_;

    ParserMany(Parser<T> *p, QList<T> *out = nullptr) : Parser< QList<T> >(out), p_(p) {}
    virtual ~ParserMany() {delete p_;}

    QList<T> parse(Input &input) {
        QList<T> result;
        try {
            Q_FOREVER {
                result.push_back(p_->parse(input));
            }
        }
        catch (const ParserException &) {
            if (Parser< QList<T> >::out_)
                *Parser< QList<T> >::out_ = result;
            return result;
        }
    }
};

template<>
struct ParserMany<QChar> : Parser<QString> {
    Parser<QChar> *p_;

    ParserMany(Parser<QChar> *p, QString *out = nullptr) : Parser(out), p_(p) {}
    virtual ~ParserMany() {delete p_;}

    QString parse(Input &input) {
        QString result;
        try {
            Q_FOREVER {
                result += p_->parse(input);
            }
        }
        catch (const ParserException &) {
            if (out_)
                *out_ = result;
            return result;
        }
    }
};

template<typename T>
struct ParserMany1 : ParserMany<T> {
    ParserMany1(Parser<T> *p, QList<T> *out = nullptr) : ParserMany<T>(p, out) {}

    QList<T> parse(Input &input) {
        QList<T> result;
        result.push_back(ParserMany<T>::p_->parse(input));
        result.append(ParserMany<T>::parse(input));

        if (ParserMany<T>::out_)
            *ParserMany<T>::out_ = result;
        return result;
    }
};

template<>
struct ParserMany1<QChar> : ParserMany<QChar> {
    ParserMany1(Parser<QChar> *p, QString *out = nullptr) : ParserMany<QChar>(p, out) {}

    QString parse(Input &input) {
        QString result;
        result += ParserMany<QChar>::p_->parse(input);
        result += ParserMany<QChar>::parse(input);

        if (out_)
            *out_ = result;
        return result;
    }
};

template<typename T>
struct ParserSkipMany : Parser<void> {
    Parser<T> *p_;

    ParserSkipMany(Parser<T> *p) : p_(p) {}
    virtual ~ParserSkipMany() {delete p_;}

    void parse(Input &input) {
        try {
            Q_FOREVER {
                p_->parse(input);
            }
        }
        catch (const ParserException &) {
        }
    }
};

template<typename T>
struct ParserSkipMany1 : ParserSkipMany<T> {
    ParserSkipMany1(Parser<T> *p) : ParserSkipMany<T>(p) {}

    void parse(Input &input) {
        ParserSkipMany<T>::p_->parse(input);
        ParserSkipMany<T>::parse(input);
    }
};

// todo: use variadic template class
template<typename T>
struct ParserChoice : Parser<T> {
  QList< Parser<T>* > ps_;

  ParserChoice(QList< Parser<T>* > ps, T *out) : Parser<T>(out), ps_(ps) {}
  ParserChoice(std::initializer_list< Parser<T>* > &ps, T *out) : Parser<T>(out), ps_(ps) {}
  virtual ~ParserChoice() {
      Q_FOREACH(auto &p, ps_) {
          delete p;
      }
      ps_.clear();
  }

  T parse(Input &input) {
      ParserException exp(0, "");
      Q_FOREACH(Parser<T> *p, ps_) {
          try {
              T result = p->parse(input);
              if(Parser<T>::out_)
                  *Parser<T>::out_ = result;
              return result;
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

    ParserSepBy(Parser<T> *p, Parser<TSep> *sep, QList<T> *out) : Parser< QList<T> >(out), p_(p), sep_(sep) {}
    virtual ~ParserSepBy() {
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

            if (Parser< QList<T> >::out_)
                *Parser< QList<T> >::out_ = result;
            return result;
        }
    }
};

template<typename T, typename TSep>
struct ParserSepBy1 : ParserSepBy<T, TSep> {
    ParserSepBy1(Parser<T> *p, Parser<TSep> *sep, QList<T> *out) : ParserSepBy<T, TSep>(p, sep, out) {}

    QList<T> parse(Input &input) {
        QList<T> result;
        result += ParserSepBy<T, TSep>::p_->parse(input);

        try {
            ParserSepBy<T, TSep>::sep_->parse(input);
        }
        catch (const ParserException &) {
            if (ParserSepBy<T, TSep>::out_)
                *ParserSepBy<T, TSep>::out_ = result;
            return result;
        }

        result += ParserSepBy<T, TSep>::parse(input);

        if (ParserSepBy<T, TSep>::out_)
            *ParserSepBy<T, TSep>::out_ = result;
        return result;
    }
};


template<typename T>
ParserMany<T> *Many(Parser<T> *p, QList<T> *out = nullptr)
{ return new ParserMany<T>(p, out); }
ParserMany<QChar> *Many(Parser<QChar> *p, QString *out = nullptr)
{ return new ParserMany<QChar>(p, out); }

template<typename T>
ParserMany1<T> *Many1(Parser<T> *p, QList<T> *out = nullptr)
{ return new ParserMany1<T>(p, out); }
ParserMany1<QChar> *Many1(Parser<QChar> *p, QString *out = nullptr)
{ return new ParserMany1<QChar>(p, out); }

template<typename T>
ParserSkipMany<T> *SkipMany(Parser<T> *p)
{ return new ParserSkipMany<T>(p); }

template<typename T>
ParserSkipMany1<T> *SkipMany1(Parser<T> *p)
{ return new ParserSkipMany1<T>(p); }

template<typename T>
ParserChoice<T> *Choice(QList< Parser<T>* > p, T *out = nullptr)
{ return new ParserChoice<T>(p, out); }

template<typename T> ParserChoice<T>
*Choice(std::initializer_list< Parser<T>* > p, T *out = nullptr)
{ return new ParserChoice<T>(p, out); }

template<typename T, typename TSep>
ParserSepBy<T, TSep> *SepBy(Parser<T> *p, Parser<TSep> *psep, QList<T> *out = nullptr)
{ return new ParserSepBy<T, TSep>(p, psep, out); }

template<typename T, typename TSep>
ParserSepBy1<T, TSep> *SepBy1(Parser<T> *p, Parser<TSep> *psep, QList<T> *out = nullptr)
{ return new ParserSepBy1<T, TSep>(p, psep, out); }

#endif // QPARSECCOMBINATOR_H
