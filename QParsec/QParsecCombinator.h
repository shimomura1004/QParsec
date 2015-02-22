#ifndef QPARSECCOMBINATOR_H
#define QPARSECCOMBINATOR_H

#include "QParsec.h"
#include <QList>

// todo: use sharedpointer for all parsers

namespace qparsec {
namespace combinators {

template<typename T>
struct ParserMany : Parser< QList<T> > {
    Parser<T> *p_;

    ParserMany(Parser<T> *p) : Parser< QList<T> >(), p_(p) {}
    virtual ~ParserMany() {delete p_;}

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

template<>
struct ParserMany<QChar> : Parser<QString> {
    Parser<QChar> *p_;

    ParserMany(Parser<QChar> *p) : Parser(), p_(p) {}
    virtual ~ParserMany() {delete p_;}

    QString parse(Input &input) {
        QString result;
        try {
            Q_FOREVER {
                result += p_->parse(input);
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

template<>
struct ParserMany1<QChar> : ParserMany<QChar> {
    ParserMany1(Parser<QChar> *p) : ParserMany<QChar>(p) {}

    QString parse(Input &input) {
        QString result;
        result += ParserMany<QChar>::p_->parse(input);
        result += ParserMany<QChar>::parse(input);

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

template<typename T>
struct ParserChoice : Parser<T> {
  QList< Parser<T>* > ps_;

  ParserChoice(QList< Parser<T>* > ps) : Parser<T>(), ps_(ps) {}
  ParserChoice(std::initializer_list< Parser<T>* > &ps) : Parser<T>(), ps_(ps) {}
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

    ParserSepBy(Parser<T> *p, Parser<TSep> *sep) : Parser< QList<T> >(), p_(p), sep_(sep) {}
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
            return result;
        }

        Q_FOREVER {
            try {
                sep_->parse(input);
            }
            catch (const ParserException &) {
                return result;
            }

            result.push_back(p_->parse(input));
        }
    }
};

template<typename T, typename TSep>
struct ParserSepBy1 : Parser< QList<T> > {
    Parser<T> *p_;
    Parser<TSep> *sep_;

    ParserSepBy1(Parser<T> *p, Parser<TSep> *sep) : Parser< QList<T> >(), p_(p), sep_(sep) {}
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
                return result;
            }

            result.push_back(p_->parse(input));
        }
    }
};

template<typename T, typename TSep>
struct ParserEndBy : Parser< QList<T> > {
    Parser<T> *p_;
    Parser<TSep> *sep_;

    ParserEndBy(Parser<T> *p, Parser<TSep> *sep) : Parser< QList<T> >(), p_(p), sep_(sep) {}
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
                return result;
            }
            sep_->parse(input);
        }
    }
};

template<typename T, typename TSep>
struct ParserEndBy1 : Parser< QList<T> > {
    Parser<T> *p_;
    Parser<TSep> *sep_;

    ParserEndBy1(Parser<T> *p, Parser<TSep> *sep) : Parser< QList<T> >(), p_(p), sep_(sep) {}
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
                return result;
            }
            sep_->parse(input);
        }
    }
};

template<typename T>
struct ParserCount : Parser< QList<T> > {
    Parser<T> *p_;
    int n_;

    ParserCount(Parser<T> *p, int n) : Parser< QList<T> >(), p_(p), n_(n) {}
    virtual ~ParserCount() { delete p_; }

    QList<T> parse(Input &input) {
        QList<T> result;

        for(int i = 0; i < n_; i++) {
            result.push_back(p_->parse(input));
        }
        return result;
    }
};

template<>
struct ParserCount<QChar> : Parser<QString> {
    Parser<QChar> *p_;
    int n_;

    ParserCount(Parser<QChar> *p, int n) : Parser<QString>(), p_(p), n_(n) {}
    virtual ~ParserCount() { delete p_; }

    QString parse(Input &input) {
        QString result;

        for(int i = 0; i < n_; i++) {
            result.push_back(p_->parse(input));
        }
        return result;
    }
};

template<typename T, typename TOpen, typename TClose>
struct ParserBetween : Parser<T> {
    Parser<T> *p_;
    Parser<TOpen> *popen_;
    Parser<TClose> *pclose_;

    ParserBetween(Parser<T> *p, Parser<TOpen> *popen, Parser<TClose> *pclose) : Parser<T>(), p_(p), popen_(popen), pclose_(pclose) {}
    virtual ~ParserBetween() {
        delete p_;
        delete popen_;
        delete pclose_;
    }

    T parse(Input &input) {
        popen_->parse(input);
        T result = p_->parse(input);
        pclose_->parse(input);

        return result;
    }
};

template<typename T>
struct ParserOption : Parser<T> {
    Parser<T> *p_;
    T opt_;

    ParserOption(Parser<T> *p, T opt) : Parser<T>(), p_(p), opt_(opt) {}
    virtual ~ParserOption() { delete p_; }

    T parse(Input &input) {
        try {
            T result = p_->parse(input);
            return result;
        }
        catch (const ParserException &) {
            return opt_;
        }
    }
};
template<typename T>
struct ParserOption_ : Parser<void> {
    Parser<T> *p_;

    ParserOption_(Parser<T> *p) : Parser<void>(), p_(p) {}
    virtual ~ParserOption_() { delete p_; }

    void parse(Input &input) {
        try { p_->parse(input); }
        catch (const ParserException &) {}
    }
};

struct ParserEof : Parser<void> {
    void parse(Input &input) {
        if (!input.isEmpty())
            throw ParserException(input.index(), "Isn't at the end of input");
    }
};

template<typename T, typename TEnd>
struct ParserManyTill : Parser< QList<T> > {
    Parser<T> *p_;
    Parser<TEnd> *pend_;

    ParserManyTill(Parser<T> *p, Parser<TEnd> *pend) : Parser< QList<T> >(), p_(p), pend_(pend) {}
    virtual ~ParserManyTill(){
        delete p_;
        delete pend_;
    }

    QList<T> parse(Input &input) {
        QList<T> result;

        Q_FOREVER {
            try {
                pend_->parse(input);
                return result;
            }
            catch (const ParserException &) {
                result.push_back(p_->parse(input));
            }
        }
    }
};

template<typename T>
struct ParserChainl : Parser<T> {
    Parser<T> *p_;
    Parser<T(*)(T, T)> *poperator_;
    T opt_;

    ParserChainl(Parser<T> *p, Parser<T(*)(T, T)> *poperator, T opt) : Parser<T>(), p_(p), poperator_(poperator), opt_(opt) {}
    ~ParserChainl() {
        delete p_;
        delete poperator_;
    }

    T parse(Input &input) {
        T accum;
        T(*op)(T, T);

        try {
            accum = p_->parse(input);
        }
        catch (const ParserException &) {
            return opt_;
        }

        Q_FOREVER {
            try {
                op = poperator_->parse(input);
            }
            catch (const ParserException &) {
                return accum;
            }

            T t = p_->parse(input);
            accum = op(accum, t);
        }
    }
};

template<typename T>
struct ParserChainl1 : Parser<T> {
    Parser<T> *p_;
    Parser<T(*)(T, T)> *poperator_;

    ParserChainl1(Parser<T> *p, Parser<T(*)(T, T)> *poperator) : Parser<T>(), p_(p), poperator_(poperator) {}
    ~ParserChainl1() {
        delete p_;
        delete poperator_;
    }

    T parse(Input &input) {
        T accum = p_->parse(input);
        T(*op)(T, T);

        Q_FOREVER {
            try {
                op = poperator_->parse(input);
            }
            catch (const ParserException &) {
                return accum;
            }

            T t = p_->parse(input);
            accum = op(accum, t);
        }
    }
};

template<typename T>
struct ParserChainr : Parser<T> {
    Parser<T> *p_;
    Parser<T(*)(T, T)> *poperator_;
    T opt_;

    ParserChainr(Parser<T> *p, Parser<T(*)(T, T)> *poperator, T opt) : Parser<T>(), p_(p), poperator_(poperator), opt_(opt) {}
    ~ParserChainr() {
        delete p_;
        delete poperator_;
    }

    T parse(Input &input) {
        T left;
        T(*op)(T, T);

        try {
            left = p_->parse(input);
        }
        catch (const ParserException &) {
            return opt_;
        }

        try {
            op = poperator_->parse(input);
        }
        catch (const ParserException &) {
            return left;
        }

        return op(left, parse(input));
    }
};

template<typename T>
struct ParserChainr1 : Parser<T> {
    Parser<T> *p_;
    Parser<T(*)(T, T)> *poperator_;

    ParserChainr1(Parser<T> *p, Parser<T(*)(T, T)> *poperator) : Parser<T>(), p_(p), poperator_(poperator) {}
    ~ParserChainr1() {
        delete p_;
        delete poperator_;
    }

    T parse(Input &input) {
        T left;
        T(*op)(T, T);

        left = p_->parse(input);

        try {
            op = poperator_->parse(input);
        }
        catch (const ParserException &) {
            return left;
        }

        return op(left, parse(input));
    }
};

template<typename T>
struct ParserLookAhead : Parser<T> {
    Parser<T> *p_;

    ParserLookAhead(Parser<T> *p) : Parser<T>(), p_(p) {}
    ~ParserLookAhead() { delete p_; }

    T parse(Input &input) {
        input.preserve();
        auto result = p_->parse(input);
        input.restore();
        return result;
    }
};

template<typename T>
ParserMany<T> *Many(Parser<T> *p)
{ return new ParserMany<T>(p); }
ParserMany<QChar> *Many(Parser<QChar> *p)
{ return new ParserMany<QChar>(p); }

template<typename T>
ParserMany1<T> *Many1(Parser<T> *p)
{ return new ParserMany1<T>(p); }
ParserMany1<QChar> *Many1(Parser<QChar> *p)
{ return new ParserMany1<QChar>(p); }

template<typename T>
ParserSkipMany<T> *SkipMany(Parser<T> *p)
{ return new ParserSkipMany<T>(p); }

template<typename T>
ParserSkipMany1<T> *SkipMany1(Parser<T> *p)
{ return new ParserSkipMany1<T>(p); }

template<typename T>
ParserChoice<T> *Choice(QList< Parser<T>* > p)
{ return new ParserChoice<T>(p); }

template<typename T>
ParserChoice<T> *Choice(std::initializer_list< Parser<T>* > ps)
{ return new ParserChoice<T>(ps); }

template<typename T, typename TSep>
ParserSepBy<T, TSep> *SepBy(Parser<T> *p, Parser<TSep> *psep)
{ return new ParserSepBy<T, TSep>(p, psep); }

template<typename T, typename TSep>
ParserSepBy1<T, TSep> *SepBy1(Parser<T> *p, Parser<TSep> *psep)
{ return new ParserSepBy1<T, TSep>(p, psep); }

template<typename T, typename TSep>
ParserEndBy<T, TSep> *EndBy(Parser<T> *p, Parser<TSep> *psep)
{ return new ParserEndBy<T, TSep>(p, psep); }

template<typename T, typename TSep>
ParserEndBy1<T, TSep> *EndBy1(Parser<T> *p, Parser<TSep> *psep)
{ return new ParserEndBy1<T, TSep>(p, psep); }

template<typename T>
ParserCount<T> *Count(Parser<T> *p, int n)
{ return new ParserCount<T>(p, n); }
ParserCount<QChar> *Count(Parser<QChar> *p, int n)
{ return new ParserCount<QChar>(p, n); }

template<typename T, typename TOpen, typename TClose>
ParserBetween<T, TOpen, TClose> *Between(Parser<T> *p, Parser<TOpen> *popen, Parser<TClose> *pclose)
{ return new ParserBetween<T, TOpen, TClose>(p, popen, pclose); }

template<typename T>
ParserOption<T> *Option(Parser<T> *p, T opt)
{ return new ParserOption<T>(p, opt); }

template<typename T>
Parser<void> *Option_(Parser<T> *p)
{ return new ParserOption_<T>(p); }

template<typename T, typename TEnd>
ParserManyTill<T, TEnd> *ManyTill(Parser<T> *p, Parser<TEnd> *pend)
{ return new ParserManyTill<T, TEnd>(p, pend); }

ParserEof *Eof()
{ return new ParserEof(); }

template<typename T>
ParserChainl<T> *Chainl(Parser<T> *p, Parser<T(*)(T, T)> *poperator, T opt)
{ return new ParserChainl<T>(p, poperator, opt); }

template<typename T>
ParserChainl1<T> *Chainl1(Parser<T> *p, Parser<T(*)(T, T)> *poperator)
{ return new ParserChainl1<T>(p, poperator); }

template<typename T>
ParserChainr<T> *Chainr(Parser<T> *p, Parser<T(*)(T, T)> *poperator, T opt)
{ return new ParserChainr<T>(p, poperator, opt); }

template<typename T>
ParserChainr1<T> *Chainr1(Parser<T> *p, Parser<T(*)(T, T)> *poperator)
{ return new ParserChainr1<T>(p, poperator); }

template<typename T>
ParserLookAhead<T> *LookAhead(Parser<T> *p)
{ return new ParserLookAhead<T>(p); }

}
}

#endif // QPARSECCOMBINATOR_H
