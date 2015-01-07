#ifndef AST_H
#define AST_H

#include <QSharedPointer>
#include <QPair>
#include <QString>

namespace ast {

struct Val {
    virtual ~Val(){}
    virtual QString toString() = 0;
};

typedef QSharedPointer<Val> SharedVal;
typedef QList<QPair<QString, SharedVal>> Env;

struct Symbol : Val {
    QString val;
    static SharedVal create(QString s) { return QSharedPointer<Symbol>(new Symbol(s)); }
    Symbol(QString s) : val(s) {}
    QString toString() {
        return val;
    }
};

struct Variable : Val {
    QString val;
    static SharedVal create(QString s) { return QSharedPointer<Variable>(new Variable(s)); }
    Variable(QString s) : val(s) {}
    QString toString() {
        return val;
    }
};

struct Char : Val {
    QChar val;
    static SharedVal create(QChar c) { return QSharedPointer<Char>(new Char(c)); }
    Char(QChar c) : val(c) {}
    QString toString() {
        return QStringLiteral("#\\%1").arg(val);
    }
};

struct String : Val {
    QString val;
    static SharedVal create(QString s) { return QSharedPointer<String>(new String(s)); }
    String(QString s) : val(s) {}
    QString toString() {
        return QStringLiteral("\"%1\"").arg(val);
    }
};

struct Bool : Val {
    bool val;
    static SharedVal create(bool b) { return QSharedPointer<Bool>(new Bool(b)); }
    Bool(bool b) : val(b) {}
    QString toString() {
        return QStringLiteral("#%1").arg(val ? "t" : "f");
    }
};

struct Integer : Val {
    int64_t val;
    static SharedVal create(int64_t i) { return QSharedPointer<Integer>(new Integer(i)); }
    Integer(int i) : val(i) {}
    QString toString() {
        return QString::number(val);
    }
};

struct Real : Val {
    double val;
    static SharedVal create(double d) { return QSharedPointer<Real>(new Real(d)); }
    Real(double d) : val(d) {}
    QString toString() {
        return QStringLiteral("%1").arg(val, 0, 'e');
    }
};

struct Rational : Val {
    int64_t numerator;
    uint64_t denominator;
    static SharedVal create(int64_t n, uint64_t d) { return QSharedPointer<Rational>(new Rational(n, d)); }
    Rational(int64_t n, uint64_t d) : numerator(n), denominator(d) {}
    QString toString() {
        return QStringLiteral("%1/%2").arg(numerator).arg(denominator);
    }
};

struct Complex : Val {
    double real;
    double imagnary;
    static SharedVal create(double r, double i) { return QSharedPointer<Complex>(new Complex(r, i)); }
    Complex(double r, double i) : real(r), imagnary(i) {}
    QString toString() {
        if (imagnary > 0.0)
            return QStringLiteral("%1+%2i").arg(real).arg(imagnary);
        return QStringLiteral("%1%2i").arg(real).arg(imagnary);
    }
};

struct Undef : Val {
    static SharedVal create() { return QSharedPointer<Undef>(new Undef()); }
    QString toString() {
        return QStringLiteral("#<undef>");
    }
};

struct Quote : Val {
    SharedVal val;
    static SharedVal create(SharedVal v) { return QSharedPointer<Quote>(new Quote(v)); }
    Quote(SharedVal v) : val(v) {}
    QString toString() {
        // todo
        return QStringLiteral("%1").arg(val->toString());
    }
};

struct Set : Val {
    QString var;
    SharedVal exp;
    static SharedVal create(QString v, SharedVal e) { return QSharedPointer<Set>(new Set(v, e)); }
    Set(QString v, SharedVal e) : var(v), exp(e) {}
    QString toString() {
        return QStringLiteral("set! %1 %2)").arg(var, exp->toString());
    }
};
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

        return QStringLiteral("(%1)").arg(result.join(" "));
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

        return QStringLiteral("(%1 . %2)").arg(result.join(" "), cdr->toString());
    }
};

struct Vector : Val {
    QList<SharedVal> elems;
    static SharedVal create(QList<SharedVal> e) { return QSharedPointer<Vector>(new Vector(e)); }
    Vector(QList<SharedVal> e) : elems(e) {}
    QString toString() {
        QStringList result;
        Q_FOREACH(const SharedVal& e, elems)
            result.push_back(e->toString());

        return QStringLiteral("#(%1)").arg(result.join(" "));
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
        return QStringLiteral("(%1 (%2))").arg(proc->toString(), result.join(" "));
    }
};

struct Lambda : Val {
    QStringList args;
    QString listarg;
    QList<SharedVal> bodies;
    Env env;
    static SharedVal create(QList<QString> a, QString l, QList<SharedVal> b, Env e) { return QSharedPointer<Lambda>(new Lambda(a, l, b, e)); }
    Lambda(QList<QString> a, QString l, QList<SharedVal> b, Env e) : args(a), listarg(l), bodies(b), env(e) {}
    QString toString() {
        QStringList bs;
        Q_FOREACH(const auto& b, bodies)
            bs.push_back(b->toString());

        // todo
        if (listarg.isEmpty())
            return QStringLiteral("(lambda (%1) %2)").arg(args.join(" "), bs.join(" "));
        return QStringLiteral("(lambda (%1 . %2)  %3)").arg(args.join(" "), listarg, bs.join(" "));
    }
};

struct Define : Val {
    QString name;
    SharedVal body;

    static SharedVal create(QString n, SharedVal b) { return QSharedPointer<Define>(new Define(n, b)); }
    Define(QString n, SharedVal b) : name(n), body(b) {}
    QString toString() {
        return QStringLiteral("(define %1 %2)").arg(name, body->toString());
    }
};

struct If : Val {
    SharedVal condition;
    SharedVal left;
    SharedVal right;
    static SharedVal create(SharedVal c, SharedVal l, SharedVal r) { return QSharedPointer<If>(new If(c, l, r)); }
    If(SharedVal c, SharedVal l, SharedVal r) : condition(c), left(l), right(r) {}
    QString toString() {
        return QStringLiteral("(if %1 %2 %3)").arg(condition->toString(), left->toString(), right->toString());
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
        return QStringLiteral("(%1)").arg(result.join(" "));
    }
};

}

#endif // AST_H
