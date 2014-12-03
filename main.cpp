#include <QCoreApplication>
#include <QString>
#include <QList>
#include <QSet>
#include <QDebug>

struct Input {
    int index;
    QString value;
    Input(QString v) : index(0), value(v) {}
};

struct ParserException {
    int index;
    QString reason;
    ParserException(int i, QString s) : index(i), reason(s) {}
};

template<typename R>
struct Parser {
    virtual R parse(Input &input) = 0;
    virtual ~Parser() {}
};

struct Ch : Parser<QChar> {
    QChar c_;
    Ch(char c) : c_(c) {}
    Ch(QChar c) : c_(c) {}
    QChar parse(Input &input) {
        if (input.value.isEmpty())
            throw ParserException(0, "Empty input");
        if (input.value[0] != c_)
            throw ParserException(input.index, QStringLiteral("Expected '%1' but got '%2'").arg(QString(c_), QString(input.value[0])));

        input.value.remove(0, 1);
        input.index += 1;
        return c_;
    }
};

template<typename R>
struct Many : Parser< QList<R> > {
    Parser<R> *p_;
    Many(Parser<R> *p) : p_(p) {}
    ~Many() {delete p_;}
    QList<R> parse(Input &input) {
        QList<R> result;
        try {
            while(true) {
                R r = p_->parse(input);
                result.push_back(r);
            }
        }
        catch (const ParserException &) {
            return result;
        }
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
struct OneOf : Parser<R> {
  QList< Parser<R>* > ps_;
  OneOf(QList< Parser<R>* > ps) : ps_(ps) {}
  ~OneOf() {
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

int main() {
    Input input("(10,01)");

    // tuple parser
    try {
        Ch('(').parse(input);
        auto num1 = QList< Parser<QChar>* >() << new Ch('0') << new Ch('1');
        auto left = Many<QChar>(new OneOf<QChar>(num1)).parse(input);
        Ch(',').parse(input);
        auto num2 = QList< Parser<QChar>* >() << new Ch('0') << new Ch('1');
        auto right = Many<QChar>(new OneOf<QChar>(num2)).parse(input);
        Ch(')').parse(input);

        qDebug() << "ParseResult:" << left << right;
        qDebug() << "Input:" << input.value;
    }
    catch (const ParserException& e) {
        qDebug() << "ParseError at" << e.index << ":" << e.reason;
    }

    return 0;
}
