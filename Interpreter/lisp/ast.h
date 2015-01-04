#ifndef AST_H
#define AST_H

#include <QSharedPointer>
#include <QPair>
#include <QString>

namespace lisp {
namespace ast {

struct Val {
    virtual ~Val(){}
    virtual QString toString() = 0;
};

typedef QSharedPointer<lisp::ast::Val> SharedVal;
typedef QList<QPair<QString, SharedVal>> Env;

struct Symbol : Val {
    QString val;
    static SharedVal create(QString s) { return QSharedPointer<Symbol>(new Symbol(s)); }
    Symbol(QString s) : val(s) {}
    QString toString() {
        return QStringLiteral("<Symbol:\"%1\">").arg(val);
    }
};

struct Var : Val {
    QString val;
    static SharedVal create(QString s) { return QSharedPointer<Var>(new Var(s)); }
    Var(QString s) : val(s) {}
    QString toString() {
        return QStringLiteral("<Var:\"%1\">").arg(val);
    }
};

struct Char : Val {
    QChar val;
    static SharedVal create(QChar c) { return QSharedPointer<Char>(new Char(c)); }
    Char(QChar c) : val(c) {}
    QString toString() {
        return QStringLiteral("<Char:'%1'>").arg(val);
    }
};

struct String : Val {
    QString val;
    static SharedVal create(QString s) { return QSharedPointer<String>(new String(s)); }
    String(QString s) : val(s) {}
    QString toString() {
        return QStringLiteral("<String:\"%1\">").arg(val);
    }
};

struct Bool : Val {
    bool val;
    static SharedVal create(bool b) { return QSharedPointer<Bool>(new Bool(b)); }
    Bool(bool b) : val(b) {}
    QString toString() {
        return QStringLiteral("<Bool:%1>").arg(val ? "true" : "false");
    }
};

struct Int : Val {
    int64_t val;
    static SharedVal create(int64_t i) { return QSharedPointer<Int>(new Int(i)); }
    Int(int i) : val(i) {}
    QString toString() {
        return QStringLiteral("<Int:%1>").arg(val);
    }
};

struct Real : Val {
    double val;
    static SharedVal create(double d) { return QSharedPointer<Real>(new Real(d)); }
    Real(double d) : val(d) {}
    QString toString() {
        return QStringLiteral("<Real:%1>").arg(val);
    }
};

struct Rational : Val {
    int64_t numerator;
    uint64_t denominator;
    static SharedVal create(int64_t n, uint64_t d) { return QSharedPointer<Rational>(new Rational(n, d)); }
    Rational(int64_t n, uint64_t d) : numerator(n), denominator(d) {}
    QString toString() {
        return QStringLiteral("<Rational:%1/%2>").arg(numerator, denominator);
    }
};

struct Complex : Val {
    double real;
    double imagnary;
    static SharedVal create(double r, double i) { return QSharedPointer<Complex>(new Complex(r, i)); }
    Complex(double r, double i) : real(r), imagnary(i) {}
    QString toString() {
        return QStringLiteral("<Complex:%1+%2i>").arg(real, imagnary);
    }
};

struct Undef : Val {
    static SharedVal create() { return QSharedPointer<Undef>(new Undef()); }
    QString toString() {
        return QStringLiteral("<Undefined>");
    }
};

struct List : Val {
    QList<SharedVal> val;
    static SharedVal create(QList<SharedVal> l) { return QSharedPointer<List>(new List(l)); }
    List(QList<SharedVal> l) : val(l) {}
    QString toString() {
        QStringList result;
        Q_FOREACH(const SharedVal& v, val)
            result.push_back(v->toString());

        return QStringLiteral("<List:(%1)>").arg(result.join(", "));
    }
};

struct DList : Val {
    QList<SharedVal> cars;
    SharedVal cdr;
    static SharedVal create(QList<SharedVal> l, SharedVal d) { return QSharedPointer<DList>(new DList(l, d)); }
    DList(QList<SharedVal> a, SharedVal d) : cars(a), cdr(d) {}
    QString toString() {
        QStringList result;
        Q_FOREACH(const SharedVal& car, cars)
            result.push_back(car->toString());

        return QStringLiteral("<DList:(%1 | %2)>").arg(result.join(", "), cdr->toString());
    }
};

struct Apply : Val {
    SharedVal proc;
    QList<SharedVal> args;
    static SharedVal create(SharedVal p, QList<SharedVal> a) { return QSharedPointer<Apply>(new Apply(p, a)); }
    Apply(SharedVal p, QList<SharedVal> a) : proc(p), args(a) {}
    QString toString() {
        QStringList result;
        Q_FOREACH(const SharedVal& arg, args)
            result.push_back(arg->toString());
        return QStringLiteral("<Apply:%1(%2)>").arg(proc->toString(), result.join(", "));
    }
};

struct Lambda : Val {
    QStringList args;
    SharedVal body;
    Env env;
    static SharedVal create(QList<QString> a, SharedVal b, Env e) { return QSharedPointer<Lambda>(new Lambda(a, b, e)); }
    Lambda(QList<QString> a, SharedVal b, Env e) : args(a), body(b), env(e) {}
    QString toString() {
        QStringList envs;
        Q_FOREACH(const auto& e, env)
            envs.push_back(QStringLiteral("%1=%2").arg(e.first, e.second->toString()));
        return QStringLiteral("<Lambda:(%1) -> %2 [%3]>").arg(args.join(", "), body->toString(), envs.join(", "));
    }
};

struct Define : Val {
    QString name;
    SharedVal body;

    static SharedVal create(QString n, SharedVal b) { return QSharedPointer<Define>(new Define(n, b)); }
    Define(QString n, SharedVal b) : name(n), body(b) {}
    QString toString() {
        return QStringLiteral("<Define:%1=%2>").arg(name, body->toString());
    }
};

struct If : Val {
    SharedVal condition;
    SharedVal left;
    SharedVal right;
    static SharedVal create(SharedVal c, SharedVal l, SharedVal r) { return QSharedPointer<If>(new If(c, l, r)); }
    If(SharedVal c, SharedVal l, SharedVal r) : condition(c), left(l), right(r) {}
    QString toString() {
        return QStringLiteral("<If:(%1,%2,%3)>").arg(condition->toString(), left->toString(), right->toString());
    }
};

struct Let : Val {
    QList<QPair<QString, SharedVal>> letenv;
    SharedVal body;
    static SharedVal create(QList<QPair<QString, SharedVal>> l, SharedVal b) { return QSharedPointer<Let>(new Let(l, b)); }
    Let(QList<QPair<QString, SharedVal>> l, SharedVal b) : letenv(l), body(b) {}
    QString toString() {
        QStringList envs;
        Q_FOREACH(const auto& e, letenv)
            envs.push_back(QStringLiteral("%1=%2").arg(e.first, e.second->toString()));
        return QStringLiteral("<Let:[%1]%2>").arg(envs.join(", "), body->toString());
    }
};

struct Sequence : Val {
    QList<SharedVal> vals;
    static SharedVal create(QList<SharedVal> v) { return QSharedPointer<Sequence>(new Sequence(v)); }
    Sequence(QList<SharedVal> v) : vals(v) {}
    QString toString() {
        QStringList result;
        Q_FOREACH(const auto& v, vals)
            result.push_back(v->toString());
        return QStringLiteral("<Seq:(%1)>").arg(result.join(", "));
    }
};

}
}

#endif // AST_H
