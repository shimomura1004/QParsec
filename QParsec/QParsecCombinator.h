#ifndef QPARSECCOMBINATOR_H
#define QPARSECCOMBINATOR_H

#include "QParsec.h"
#include <QList>

// todo: use sharedpointer for all parsers

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
            return Parser< QList<T> >::setOut(result);
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
            return setOut(result);
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

        return ParserMany<T>::setOut(result);
    }
};

template<>
struct ParserMany1<QChar> : ParserMany<QChar> {
    ParserMany1(Parser<QChar> *p, QString *out = nullptr) : ParserMany<QChar>(p, out) {}

    QString parse(Input &input) {
        QString result;
        result += ParserMany<QChar>::p_->parse(input);
        result += ParserMany<QChar>::parse(input);

        return setOut(result);
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
        catch (const ParserException &) {}
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
              return Parser<T>::setOut(result);
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

        try {
            result.push_back(p_->parse(input));
        }
        catch (const ParserException &) {
            return Parser< QList<T> >::setOut(result);
        }

        Q_FOREVER {
            try {
                sep_->parse(input);
            }
            catch (const ParserException &) {
                return Parser< QList<T> >::setOut(result);
            }

            result.push_back(p_->parse(input));
        }
    }
};

template<typename T, typename TSep>
struct ParserSepBy1 : Parser< QList<T> > {
    Parser<T> *p_;
    Parser<TSep> *sep_;

    ParserSepBy1(Parser<T> *p, Parser<TSep> *sep, QList<T> *out) : Parser< QList<T> >(out), p_(p), sep_(sep) {}
    virtual ~ParserSepBy1() {
        delete p_;
        delete sep_;
    }

    QList<T> parse(Input &input) {
        QList<T> result;

        result.push_back(p_->parse(input));
        Q_FOREVER {
            try {
                sep_->parse(input);
            }
            catch (const ParserException &) {
                return Parser< QList<T> >::setOut(result);
            }

            result.push_back(p_->parse(input));
        }
    }
};

template<typename T, typename TSep>
struct ParserEndBy : Parser< QList<T> > {
    Parser<T> *p_;
    Parser<TSep> *sep_;

    ParserEndBy(Parser<T> *p, Parser<TSep> *sep, QList<T> *out) : Parser< QList<T> >(out), p_(p), sep_(sep) {}
    virtual ~ParserEndBy() {
        delete p_;
        delete sep_;
    }

    QList<T> parse(Input &input) {
        QList<T> result;

        Q_FOREVER {
            try {
                result.push_back(p_->parse(input));
            }
            catch (const ParserException &) {
                return Parser< QList<T> >::setOut(result);
            }
            sep_->parse(input);
        }
    }
};

template<typename T, typename TSep>
struct ParserEndBy1 : Parser< QList<T> > {
    Parser<T> *p_;
    Parser<TSep> *sep_;

    ParserEndBy1(Parser<T> *p, Parser<TSep> *sep, QList<T> *out) : Parser< QList<T> >(out), p_(p), sep_(sep) {}
    virtual ~ParserEndBy1() {
        delete p_;
        delete sep_;
    }

    QList<T> parse(Input &input) {
        QList<T> result;

        result.push_back(p_->parse(input));
        sep_->parse(input);

        Q_FOREVER {
            try {
                result.push_back(p_->parse(input));
            }
            catch (const ParserException &) {
                return Parser< QList<T> >::setOut(result);
            }
            sep_->parse(input);
        }
    }
};

template<typename T>
struct ParserCount : Parser< QList<T> > {
    Parser<T> *p_;
    int n_;

    ParserCount(Parser<T> *p, int n, QList<T> *out) : Parser< QList<T> >(out), p_(p), n_(n) {}
    virtual ~ParserCount() { delete p_; }

    QList<T> parse(Input &input) {
        QList<T> result;

        for(int i = 0; i < n_; i++) {
            result.push_back(p_->parse(input));
        }
        return setOut(result);
    }
};

template<>
struct ParserCount<QChar> : Parser<QString> {
    Parser<QChar> *p_;
    int n_;

    ParserCount(Parser<QChar> *p, int n, QString *out) : Parser<QString>(out), p_(p), n_(n) {}
    virtual ~ParserCount() { delete p_; }

    QString parse(Input &input) {
        QString result;

        for(int i = 0; i < n_; i++) {
            result.push_back(p_->parse(input));
        }
        return setOut(result);
    }
};

template<typename T, typename TOpen, typename TClose>
struct ParserBetween : Parser<T> {
    Parser<T> *p_;
    Parser<TOpen> *popen_;
    Parser<TClose> *pclose_;

    ParserBetween(Parser<T> *p, Parser<TOpen> *popen, Parser<TClose> *pclose, T *out) : Parser<T>(out), p_(p), popen_(popen), pclose_(pclose) {}
    virtual ~ParserBetween() {
        delete p_;
        delete popen_;
        delete pclose_;
    }

    T parse(Input &input) {
        popen_->parse(input);
        T result = p_->parse(input);
        pclose_->parse(input);

        return Parser<T>::setOut(result);
    }
};

template<typename T>
struct ParserOption : Parser<T> {
    Parser<T> *p_;
    T opt_;

    ParserOption(Parser<T> *p, T opt, T *out) : Parser<T>(out), p_(p), opt_(opt) {}
    virtual ~ParserOption() { delete p_; }

    T parse(Input &input) {
        try {
            T result = p_->parse(input);
            return Parser<T>::setOut(result);
        }
        catch (const ParserException &) {
            return Parser<T>::setOut(opt_);
        }
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
*Choice(std::initializer_list< Parser<T>* > ps, T *out = nullptr)
{ return new ParserChoice<T>(ps, out); }

template<typename T, typename TSep>
ParserSepBy<T, TSep> *SepBy(Parser<T> *p, Parser<TSep> *psep, QList<T> *out = nullptr)
{ return new ParserSepBy<T, TSep>(p, psep, out); }

template<typename T, typename TSep>
ParserSepBy1<T, TSep> *SepBy1(Parser<T> *p, Parser<TSep> *psep, QList<T> *out = nullptr)
{ return new ParserSepBy1<T, TSep>(p, psep, out); }

template<typename T, typename TSep>
ParserEndBy<T, TSep> *EndBy(Parser<T> *p, Parser<TSep> *psep, QList<T> *out = nullptr)
{ return new ParserEndBy<T, TSep>(p, psep, out); }

template<typename T, typename TSep>
ParserEndBy1<T, TSep> *EndBy1(Parser<T> *p, Parser<TSep> *psep, QList<T> *out = nullptr)
{ return new ParserEndBy1<T, TSep>(p, psep, out); }

template<typename T>
ParserCount<T> *Count(Parser<T> *p, int n, QList<T> *out = nullptr)
{ return new ParserCount<T>(p, n, out); }
ParserCount<QChar> *Count(Parser<QChar> *p, int n, QString *out = nullptr)
{ return new ParserCount<QChar>(p, n, out); }

template<typename T, typename TOpen, typename TClose>
ParserBetween<T, TOpen, TClose> *Between(Parser<T> *p, Parser<TOpen> *popen, Parser<TClose> *pclose, T *out = nullptr)
{ return new ParserBetween<T, TOpen, TClose>(p, popen, pclose, out); }

template<typename T>
ParserOption<T> *Option(Parser<T> *p, T opt, T *out = nullptr)
{ return new ParserOption<T>(p, opt, out); }

#endif // QPARSECCOMBINATOR_H