#ifndef DO_H
#define DO_H

#include "val.h"

namespace qscheme {
namespace ast {

class Do : public Val {
protected:
    struct IterationSpec {
        QString var;
        SharedVal init;
        static QSharedPointer<IterationSpec> create(QString v, SharedVal i);
        IterationSpec(QString v, SharedVal i);
        virtual ~IterationSpec();
        virtual QString toString();
    };
    struct IterationSpecWithStep : IterationSpec {
        SharedVal step;
        static QSharedPointer<IterationSpecWithStep> create(QString v, SharedVal i, SharedVal s);
        IterationSpecWithStep(QString v, SharedVal i, SharedVal s);
        virtual ~IterationSpecWithStep();
        virtual QString toString();
    };

    QList<QSharedPointer<IterationSpec>> iterationspecs;
    SharedVal test;
    SharedVal doresult;
    QList<SharedVal> commands;

    Do(QList<QSharedPointer<IterationSpec>> i, SharedVal t, SharedVal d, QList<SharedVal> c);

public:
    static SharedVal create(QList<QSharedPointer<IterationSpec>> i, SharedVal t, SharedVal d, QList<SharedVal> c);
    QString toString();
};

}
}

#endif // DO_H
