#ifndef AST_H
#define AST_H

#include <QList>
#include <QPair>
#include <QString>
#include <QSharedPointer>

namespace AST {

struct LispVal {
    virtual ~LispVal(){}
    virtual QString toString() = 0;
};

typedef QList<QPair<QString, QSharedPointer<LispVal>>> Env;

struct LispSymbol : LispVal {
    QString val;
    static QSharedPointer<LispSymbol> create(QString s) { return QSharedPointer<LispSymbol>(new LispSymbol(s)); }
    LispSymbol(QString s) : val(s) {}
    QString toString() {
        return QStringLiteral("<Symbol:\"%1\">").arg(val);
    }
};

struct LispChar : LispVal {
    QChar val;
    static QSharedPointer<LispChar> create(QChar c) { return QSharedPointer<LispChar>(new LispChar(c)); }
    LispChar(QChar c) : val(c) {}
    QString toString() {
        return QStringLiteral("<Char:'%1'>").arg(val);
    }
};

struct LispString : LispVal {
    QString val;
    static QSharedPointer<LispString> create(QString s) { return QSharedPointer<LispString>(new LispString(s)); }
    LispString(QString s) : val(s) {}
    QString toString() {
        return QStringLiteral("<String:\"%1\">").arg(val);
    }
};

struct LispBool : LispVal {
    bool val;
    static QSharedPointer<LispBool> create(bool b) { return QSharedPointer<LispBool>(new LispBool(b)); }
    LispBool(bool b) : val(b) {}
    QString toString() {
        return QStringLiteral("<Bool:%1>").arg(val ? "true" : "false");
    }
};

struct LispInt : LispVal {
    int val;
    static QSharedPointer<LispInt> create(int i) { return QSharedPointer<LispInt>(new LispInt(i)); }
    LispInt(int i) : val(i) {}
    QString toString() {
        return QStringLiteral("<Int:%1>").arg(val);
    }
};

struct LispUndef : LispVal {
    static QSharedPointer<LispUndef> create() { return QSharedPointer<LispUndef>(new LispUndef()); }
    QString toString() {
        return QStringLiteral("<Undefined>");
    }
};

struct LispList : LispVal {
    QList<QSharedPointer<LispVal>> val;
    static QSharedPointer<LispList> create(std::initializer_list<QSharedPointer<LispVal>> l) { return QSharedPointer<LispList>(new LispList(l)); }
    LispList(std::initializer_list<QSharedPointer<LispVal>> l) : val(l) {}
    QString toString() {
        QStringList result;
        Q_FOREACH(const QSharedPointer<LispVal>& v, val)
            result.push_back(v->toString());

        return QStringLiteral("<List:(%1)>").arg(result.join(", "));
    }
};

struct LispDList : LispVal {
    QList<QSharedPointer<LispVal>> cars;
    QSharedPointer<LispVal> cdr;
    static QSharedPointer<LispDList> create(std::initializer_list<QSharedPointer<LispVal>> l, QSharedPointer<LispVal> d) { return QSharedPointer<LispDList>(new LispDList(l, d)); }
    LispDList(std::initializer_list<QSharedPointer<LispVal>> a, QSharedPointer<LispVal> d) : cars(a), cdr(d) {}
    QString toString() {
        QStringList result;
        Q_FOREACH(const QSharedPointer<LispVal>& car, cars)
            result.push_back(car->toString());

        return QStringLiteral("<DList:(%1 | %2)>").arg(result.join(", "), cdr->toString());
    }
};

struct LispApply : LispVal {
    QSharedPointer<LispVal> proc;
    QList<QSharedPointer<LispVal>> args;
    static QSharedPointer<LispApply> create(QSharedPointer<LispVal> p, std::initializer_list<QSharedPointer<LispVal>> a) { return QSharedPointer<LispApply>(new LispApply(p, a)); }
    LispApply(QSharedPointer<LispVal> p, std::initializer_list<QSharedPointer<LispVal>> a) : proc(p), args(a) {}
    QString toString() {
        QStringList result;
        Q_FOREACH(const QSharedPointer<LispVal>& arg, args)
            result.push_back(arg->toString());
        return QStringLiteral("<Apply:%1(%2)>").arg(proc->toString(), result.join(", "));
    }
};

struct LispLambda : LispVal {
    QStringList args;
    QSharedPointer<LispVal> body;
    Env env;
    static QSharedPointer<LispLambda> create(std::initializer_list<QString> a, QSharedPointer<LispVal> b, Env e) { return QSharedPointer<LispLambda>(new LispLambda(a, b, e)); }
    LispLambda(std::initializer_list<QString> a, QSharedPointer<LispVal> b, Env e) : args(a), body(b), env(e) {}
    QString toString() {
        QStringList envs;
        Q_FOREACH(const auto& e, env)
            envs.push_back(QStringLiteral("%1=%2").arg(e.first, e.second->toString()));
        return QStringLiteral("<Lambda:(%1) -> %2[%3]>").arg(args.join(", "), body->toString(), envs.join(", "));
    }
};

struct LispDefine : LispVal {
    QString name;
    QSharedPointer<LispVal> body;

    static QSharedPointer<LispDefine> create(QString n, QSharedPointer<LispVal> b) { return QSharedPointer<LispDefine>(new LispDefine(n, b)); }
    LispDefine(QString n, QSharedPointer<LispVal> b) : name(n), body(b) {}
    QString toString() {
        return QStringLiteral("<Define:%1=%2>").arg(name, body->toString());
    }
};

struct LispIf : LispVal {
    QSharedPointer<LispVal> condition;
    QSharedPointer<LispVal> left;
    QSharedPointer<LispVal> right;
    static QSharedPointer<LispIf> create(QSharedPointer<LispVal> c, QSharedPointer<LispVal> l, QSharedPointer<LispVal> r) { return QSharedPointer<LispIf>(new LispIf(c, l, r)); }
    LispIf(QSharedPointer<LispVal> c, QSharedPointer<LispVal> l, QSharedPointer<LispVal> r) : condition(c), left(l), right(r) {}
    QString toString() {
        return QStringLiteral("<If:(%1,%2,%3)>").arg(condition->toString(), left->toString(), right->toString());
    }
};

struct LispLet : LispVal {
    QList<QPair<QString, QSharedPointer<LispVal>>> letenv;
    QSharedPointer<LispVal> body;
    static QSharedPointer<LispLet> create(std::initializer_list<QPair<QString, QSharedPointer<LispVal>>> l, QSharedPointer<LispVal> b) { return QSharedPointer<LispLet>(new LispLet(l, b)); }
    LispLet(std::initializer_list<QPair<QString, QSharedPointer<LispVal>>> l, QSharedPointer<LispVal> b) : letenv(l), body(b) {}
    QString toString() {
        QStringList envs;
        Q_FOREACH(const auto& e, letenv)
            envs.push_back(QStringLiteral("%1=%2").arg(e.first, e.second->toString()));
        return QStringLiteral("<Let:[%1]%2>").arg(envs.join(", "), body->toString());
    }
};

struct LispSequence : LispVal {
    QList<QSharedPointer<LispVal>> vals;
    static QSharedPointer<LispSequence> create(std::initializer_list<QSharedPointer<LispVal>> v) { return QSharedPointer<LispSequence>(new LispSequence(v)); }
    LispSequence(std::initializer_list<QSharedPointer<LispVal>> v) : vals(v) {}
    QString toString() {
        QStringList result;
        Q_FOREACH(const auto& v, vals)
            result.push_back(v->toString());
        return QStringLiteral("<Seq:(%1)>").arg(result.join(", "));
    }
};

}

#endif // AST_H
