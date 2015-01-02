#ifndef AST_H
#define AST_H

#include <QSharedPointer>
#include <QPair>

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
    static QSharedPointer<Symbol> create(QString s) { return QSharedPointer<Symbol>(new Symbol(s)); }
    Symbol(QString s) : val(s) {}
    QString toString() {
        return QStringLiteral("<Symbol:\"%1\">").arg(val);
    }
};

struct Char : Val {
    QChar val;
    static QSharedPointer<Char> create(QChar c) { return QSharedPointer<Char>(new Char(c)); }
    Char(QChar c) : val(c) {}
    QString toString() {
        return QStringLiteral("<Char:'%1'>").arg(val);
    }
};

struct String : Val {
    QString val;
    static QSharedPointer<String> create(QString s) { return QSharedPointer<String>(new String(s)); }
    String(QString s) : val(s) {}
    QString toString() {
        return QStringLiteral("<String:\"%1\">").arg(val);
    }
};

struct Bool : Val {
    bool val;
    static QSharedPointer<Bool> create(bool b) { return QSharedPointer<Bool>(new Bool(b)); }
    Bool(bool b) : val(b) {}
    QString toString() {
        return QStringLiteral("<Bool:%1>").arg(val ? "true" : "false");
    }
};

struct Int : Val {
    int val;
    static SharedVal create(int i) { return QSharedPointer<Int>(new Int(i)); }
    Int(int i) : val(i) {}
    QString toString() {
        return QStringLiteral("<Int:%1>").arg(val);
    }
};

struct Undef : Val {
    static QSharedPointer<Undef> create() { return QSharedPointer<Undef>(new Undef()); }
    QString toString() {
        return QStringLiteral("<Undefined>");
    }
};

struct List : Val {
    QList<SharedVal> val;
    static QSharedPointer<List> create(std::initializer_list<SharedVal> l) { return QSharedPointer<List>(new List(l)); }
    List(std::initializer_list<SharedVal> l) : val(l) {}
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
    static QSharedPointer<DList> create(std::initializer_list<SharedVal> l, SharedVal d) { return QSharedPointer<DList>(new DList(l, d)); }
    DList(std::initializer_list<SharedVal> a, SharedVal d) : cars(a), cdr(d) {}
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
    static QSharedPointer<Apply> create(SharedVal p, std::initializer_list<SharedVal> a) { return QSharedPointer<Apply>(new Apply(p, a)); }
    Apply(SharedVal p, std::initializer_list<SharedVal> a) : proc(p), args(a) {}
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
    static QSharedPointer<Lambda> create(QList<QString> a, SharedVal b, Env e) { return QSharedPointer<Lambda>(new Lambda(a, b, e)); }
    static QSharedPointer<Lambda> create(std::initializer_list<QString> a, SharedVal b, Env e) { return QSharedPointer<Lambda>(new Lambda(a, b, e)); }
    Lambda(QList<QString> a, SharedVal b, Env e) : args(a), body(b), env(e) {}
    Lambda(std::initializer_list<QString> a, SharedVal b, Env e) : args(a), body(b), env(e) {}
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

    static QSharedPointer<Define> create(QString n, SharedVal b) { return QSharedPointer<Define>(new Define(n, b)); }
    Define(QString n, SharedVal b) : name(n), body(b) {}
    QString toString() {
        return QStringLiteral("<Define:%1=%2>").arg(name, body->toString());
    }
};

struct If : Val {
    SharedVal condition;
    SharedVal left;
    SharedVal right;
    static QSharedPointer<If> create(SharedVal c, SharedVal l, SharedVal r) { return QSharedPointer<If>(new If(c, l, r)); }
    If(SharedVal c, SharedVal l, SharedVal r) : condition(c), left(l), right(r) {}
    QString toString() {
        return QStringLiteral("<If:(%1,%2,%3)>").arg(condition->toString(), left->toString(), right->toString());
    }
};

struct Let : Val {
    QList<QPair<QString, SharedVal>> letenv;
    SharedVal body;
    static QSharedPointer<Let> create(std::initializer_list<QPair<QString, SharedVal>> l, SharedVal b) { return QSharedPointer<Let>(new Let(l, b)); }
    Let(std::initializer_list<QPair<QString, SharedVal>> l, SharedVal b) : letenv(l), body(b) {}
    QString toString() {
        QStringList envs;
        Q_FOREACH(const auto& e, letenv)
            envs.push_back(QStringLiteral("%1=%2").arg(e.first, e.second->toString()));
        return QStringLiteral("<Let:[%1]%2>").arg(envs.join(", "), body->toString());
    }
};

struct Sequence : Val {
    QList<SharedVal> vals;
    static QSharedPointer<Sequence> create(std::initializer_list<SharedVal> v) { return QSharedPointer<Sequence>(new Sequence(v)); }
    Sequence(std::initializer_list<SharedVal> v) : vals(v) {}
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
